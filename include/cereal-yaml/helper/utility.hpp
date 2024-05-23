//
// Created by malin on 16/05/2024.
//

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <cereal/details/traits.hpp>

namespace cereal {

    //TODO take into accoun non text archives.

    //-------------------------- Setting Style Directly -------------------------------------//



    // Templated function that only allows YAMLOutputArchive.
    template<typename T>
    typename std::enable_if<std::is_same<YAMLOutputArchive, T>::value, void>::type
    Set_YAML_Style_Flow(T & ar) {
        //ar(cereal::make_size_tag(0));
        ar.Set_Style_Flow();
    };

    template<typename T>
    typename std::enable_if<!std::is_same<YAMLOutputArchive, T>::value, void>::type
    Set_YAML_Style_Flow(T & ar) {};

    // Helper function that deduces the type and calls the templated function.
    inline void Set_YAML_Style_Flow(YAMLOutputArchive & ar) {
        Set_YAML_Style_Flow<YAMLOutputArchive>(ar);
    };

    // Templated function that only allows YAMLOutputArchive.
    template<typename T>
    typename std::enable_if<std::is_same<YAMLOutputArchive, T>::value, void>::type
    Set_YAML_Style_Block(T & ar) {
        //ar(cereal::make_size_tag(0));
        ar.Set_Style_Block();
    };

    template<typename T>
    typename std::enable_if<!std::is_same<YAMLOutputArchive, T>::value, void>::type
    Set_YAML_Style_Block(T & ar) {};

    // Helper function that deduces the type and calls the templated function.
    inline void Set_YAML_Style_Block(YAMLOutputArchive & ar) {
        Set_YAML_Style_Block<YAMLOutputArchive>(ar);
    };

    //---------------------- Setting Style Flow For Next -----------------------------------//

    //-------- Iterator ---------//

    // // Customization for YAMLOutputArchive to enable flow style
    template<typename Archive_T, typename T>
    static typename std::enable_if<std::is_same<YAMLOutputArchive, Archive_T>::value, void>::type //why is it so hard to make this work?
    Format_Flow(Archive_T& archive, const char* name, T& data) {

        archive.Set_Style_Flow();
        archive.setNextName(name);
        archive.startNode();
        archive.makeArray();
        for (const auto& item : data) {
            archive(item);
        }
        archive.finishNode();

    };

    template<typename Archive_T, typename T>
    static typename std::enable_if<!std::is_same<YAMLOutputArchive, Archive_T>::value, void>::type //why is it so hard to make this work?
    Format_Flow(Archive_T& archive, const char* name, T& data) {

        archive.setNextName(name);
        for (const auto& item : data) {
            archive(item);
        }


    };

    // Helper function that deduces the type and calls the templated function.
    template<typename T>
    inline void Format_Flow(YAMLOutputArchive& archive, const char* name, T& data) {
        Format_Flow<YAMLOutputArchive>(archive, name, data);
    };

    template<class Archive, typename T> inline
    typename std::enable_if<cereal::traits::is_same_archive<Archive, YAMLOutputArchive>::value, void>::type
    Format_Flow(Archive& archive, const char* name, T& data) {
        Format_Flow<YAMLOutputArchive>(archive, name, data);
    };

    template<class Archive, typename T> inline
    CEREAL_ARCHIVE_RESTRICT(BinaryInputArchive, BinaryOutputArchive)
    Format_Flow(Archive& archive, const char* name, T& data) {
        //archive.setNextName(name);
        archive(data);
    };

    //-------- Range ---------//

    //------- Write --------//

    template<typename Archive_T, typename T>
    static typename std::enable_if<std::is_same<YAMLOutputArchive, Archive_T>::value, void>::type //why is it so hard to make this work?
    Format_Flow(Archive_T& archive, const char* name, T& data, const size_t size) {

        archive.Set_Style_Flow();
        archive.setNextName(name);
        archive.startNode();
        archive.makeArray();

        for (size_t i = 0; i < size; i++) {
            archive(data[i]);
        }

        archive.finishNode();

    };

    //------- Read --------//

    template<typename Archive_T, typename T>
    static typename std::enable_if<!std::is_same<YAMLOutputArchive, Archive_T>::value, void>::type //why is it so hard to make this work?
    Format_Flow(Archive_T& archive, const char* name, T& data, const size_t size) {


            archive.setNextName(name);

            for (size_t i = 0; i < size; i++) {
                archive(data[i]);
            }

            // archive.Set_Style_Block();

    };

    // Helper function that deduces the type and calls the templated function.
    template<typename T>
    inline void Format_Flow(YAMLOutputArchive& archive, const char* name, T& data, const size_t size) {
        Format_Flow<YAMLOutputArchive>(archive, name, data, size);
    };

    template<typename Archive_T, typename T, std::enable_if<traits::is_text_archive<Archive_T>::value>>
    inline void Format_Flow(Archive_T& archive, const char* name, T& data, const size_t size) {
        for (size_t i = 0; i < size; i++) {
            archive(data[i]);
        }
    };





}

#endif //UTILITY_HPP
