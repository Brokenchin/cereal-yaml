//
// Created by malin on 16/05/2024.
//

#ifndef UTILITY_HPP
#define UTILITY_HPP

namespace cereal {

    // Templated function that only allows YAMLOutputArchive.
    template<typename T>
    typename std::enable_if<std::is_same<YAMLOutputArchive, T>::value, void>::type
    Set_YAML_Style_Flow(T & ar) {
        //ar(cereal::make_size_tag(0));
        ar.Set_Style_Flow();
    }


    template<typename T>
    typename std::enable_if<!std::is_same<YAMLOutputArchive, T>::value, void>::type
    Set_YAML_Style_Flow(T & ar) {}

    // Helper function that deduces the type and calls the templated function.
    inline void Set_YAML_Style_Flow(YAMLOutputArchive & ar) {
        Set_YAML_Style_Flow<YAMLOutputArchive>(ar);
    }

    // Templated function that only allows YAMLOutputArchive.
    template<typename T>
    typename std::enable_if<std::is_same<YAMLOutputArchive, T>::value, void>::type
    Set_YAML_Style_Block(T & ar) {
        //ar(cereal::make_size_tag(0));
        ar.Set_Style_Block();
    }

    template<typename T>
    typename std::enable_if<!std::is_same<YAMLOutputArchive, T>::value, void>::type
    Set_YAML_Style_Block(T & ar) {}

    // Helper function that deduces the type and calls the templated function.
    inline void Set_YAML_Style_Block(YAMLOutputArchive & ar) {
        Set_YAML_Style_Block<YAMLOutputArchive>(ar);
    }

    // // Customization for YAMLOutputArchive to enable flow style
    template<typename Archive_T, typename T>
    static typename std::enable_if<std::is_same<YAMLOutputArchive, Archive_T>::value, void>::type //why is it so hard to make this work?
    Format_Flow(Archive_T& archive, const char* name, T& data, bool flow) {
        if (flow) {
            archive.Set_Style_Flow();
            archive.setNextName(name);
            archive.startNode();
            archive.makeArray();
            for (const auto& item : data) {
                archive(item);
            }
            archive.finishNode();
            // archive.Set_Style_Block();
        } else {
            archive(cereal::make_nvp("Array", data));
        }


    }

    template<typename Archive_T, typename T>
    static typename std::enable_if<!std::is_same<YAMLOutputArchive, Archive_T>::value, void>::type //why is it so hard to make this work?
    Format_Flow(Archive_T& archive, const char* name, T& data, bool flow) {

        if (flow) {
            archive.setNextName(name);
            archive.startNode();
            archive.makeArray();
            for (const auto& item : data) {
                archive(item);
            }
            archive.finishNode();
        }

    }

    // Helper function that deduces the type and calls the templated function.
    template<typename T>
    inline void Format_Flow(YAMLOutputArchive& archive, const char* name, T& data, bool flow) {
        Format_Flow<YAMLOutputArchive>(archive, name, data, flow);
    }

}

#endif //UTILITY_HPP
