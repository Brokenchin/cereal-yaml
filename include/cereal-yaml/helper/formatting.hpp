//
// Created by malin on 16/05/2024.
//

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <cereal/details/traits.hpp>

namespace cereal {

    //TODO take into accoun non text archives.
    template <typename Archive, typename = std::enable_if_t<traits::is_text_archive<Archive>::value>>
    void myMethod(Archive& archive) {
        // Your method implementation here
    }

    //-------------------------- Setting Style Directly -------------------------------------//

    template<typename Archive_T> static
    typename std::enable_if<cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type
    Set_YAML_Style_Flow(Archive_T & ar) {
        //ar(cereal::make_size_tag(0));
        ar.Set_Style_Flow();
    };

    template<typename Archive_T> static
    typename std::enable_if<!cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type
    Set_YAML_Style_Flow(Archive_T & ar) {};

    template<typename Archive_T> static
    typename std::enable_if<cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type
    Set_YAML_Style_Block(Archive_T & ar) {
        //ar(cereal::make_size_tag(0));
        ar.Set_Style_Block();
    };

    template<typename Archive_T> static
    typename std::enable_if<!cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type
    Set_YAML_Style_Block(Archive_T & ar) {};

    //---------------------- Setting Style Flow For Next -----------------------------------//

    //-------- Iterator ---------//

    //-- Write --------//

    // // Customization for YAMLOutputArchive to enable flow style
    template<typename Archive_T, typename T> static
    typename std::enable_if<cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type
    Format_Flow(Archive_T& archive, const char* name, T& data) {

        archive.Set_Style_Flow();
        archive.setNextName(name);
        archive.startNode();
        archive.makeArray();

        for (const auto& item : data)
            archive(item);

        archive.finishNode();

    };

    //-- Read --------//

    template<typename Archive_T, typename T, typename = std::enable_if_t<traits::is_text_archive<Archive_T>::value>>
    static typename std::enable_if<!cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type //why is it so hard to make this work?
    Format_Flow(Archive_T& archive, const char* name, T& data) {

        archive.setNextName(name);
        archive.startNode();
        //archive(data);
        for (auto& item : data)
            archive(item);
        archive.finishNode();
    };

    template<class Archive_T, typename T, typename = std::enable_if_t<!traits::is_text_archive<Archive_T>::value>>
    static void Format_Flow(Archive_T& archive, const char* name, T& data) {
        for (auto& item : data)
            archive(item);
    };

    //-------- Range ---------//

    //-- Write --------//

    // // Customization for YAMLOutputArchive to enable flow style
    template<typename Archive_T, typename T> static
    typename std::enable_if<cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type
    Format_Flow(Archive_T& archive, const char* name, T& data, size_t size) {

        archive.Set_Style_Flow();
        archive.setNextName(name);
        archive.startNode();
        archive.makeArray();

        for (size_t i = 0; i < size; i++)
            archive(data[i]);

        archive.finishNode();

    };

    //--  Read --------//

    template<typename Archive_T, typename T, typename = std::enable_if_t<traits::is_text_archive<Archive_T>::value>>
    static typename std::enable_if<!cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type //why is it so hard to make this work?
    Format_Flow(Archive_T& archive, const char* name, T& data, const size_t size) {

        archive.setNextName(name);
        archive.startNode();
        for (size_t i = 0; i < size; i++)
            archive(data[i]);
        archive.finishNode();

    };

    template<class Archive_T, typename T, typename = std::enable_if_t<!traits::is_text_archive<Archive_T>::value>>
    static void Format_Flow(Archive_T& archive, const char* name, T& data, const size_t size) {

        for (size_t i = 0; i < size; i++)
            archive(data[i]);
    };

}

#endif //UTILITY_HPP
