/**
 @Copyright (c) 2024, Brokenchin
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
      * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
      * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
      * Neither the name of the copyright holder nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef FORMATTING_HPP
#define FORMATTING_HPP

#include <cereal/details/traits.hpp>

namespace cereal {

    //-------------------------- Setting Style Directly -------------------------------------//



    //---------------------- Setting Style Flow For Next -----------------------------------//

    //-------- Iterator ---------//

    //-- Write --------//

    // // Customization for YAMLOutputArchive to enable flow style
    template<typename Archive_T, typename T> static
    typename std::enable_if<cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type
    Format_As_Flow(Archive_T& archive, const char* name, T& data) {

        archive.Set_Style_Flow();
        archive.setNextName(name);
        archive.startNode();
        archive.makeArray();

        for (const auto& item : data)
            archive(item);

        archive.finishNode();

        archive.Set_Style_Block();

    };

    //-- Read --------//

    template<typename Archive_T, typename T, typename = std::enable_if_t<traits::is_text_archive<Archive_T>::value>>
    static typename std::enable_if<!cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type //why is it so hard to make this work?
    Format_As_Flow(Archive_T& archive, const char* name, T& data) {

        archive.setNextName(name);
        archive.startNode();
        //archive(data);
        for (auto& item : data)
            archive(item);
        archive.finishNode();
    };

    template<class Archive_T, typename T, typename = std::enable_if_t<!traits::is_text_archive<Archive_T>::value>>
    static void Format_As_Flow(Archive_T& archive, const char* name, T& data) {
        for (auto& item : data)
            archive(item);
    };

    //-------- Range ---------//

    //-- Write --------//

    // // Customization for YAMLOutputArchive to enable flow style
    template<typename Archive_T, typename T> static
    typename std::enable_if<cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type
    Format_As_Flow(Archive_T& archive, const char* name, T& data, size_t size) {

        archive.Set_Style_Flow();
        archive.setNextName(name);
        archive.startNode();
        archive.makeArray();

        for (size_t i = 0; i < size; i++)
            archive(data[i]);

        archive.finishNode();
        archive.Set_Style_Block();

    };

    //--  Read --------//

    template<typename Archive_T, typename T, typename = std::enable_if_t<traits::is_text_archive<Archive_T>::value>>
    static typename std::enable_if<!cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type //why is it so hard to make this work?
    Format_As_Flow(Archive_T& archive, const char* name, T& data, const size_t size) {

        archive.setNextName(name);
        archive.startNode();
        for (size_t i = 0; i < size; i++)
            archive(data[i]);
        archive.finishNode();

    };

    //--  Binary --------//

    template<class Archive_T, typename T, typename = std::enable_if_t<!traits::is_text_archive<Archive_T>::value>>
    static void Format_As_Flow(Archive_T& archive, const char* name, T& data, const size_t size) {

        //archive( make_size_tag( static_cast<size_type>(size) ) );
        for (size_t i = 0; i < size; i++)
            archive(data[i]);
    };

    //---------------------- Setting Style Block For Next -----------------------------------//

    //--  Write --------//

    template<typename Archive_T> static
    typename std::enable_if<cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type
    Format_As_Group(Archive_T& archive, const char* name, const std::function<void()> & fn) {

        archive.setNextName(name);
        archive.startNode();

        fn();

        archive.finishNode();

    };

    //--  Read --------//

    template<typename Archive_T, typename = std::enable_if_t<traits::is_text_archive<Archive_T>::value>>
    static typename std::enable_if<!cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type //why is it so hard to make this work?
    Format_As_Group(Archive_T& archive, const char* name, std::function<void()> & fn) {

        archive.setNextName(name);
        archive.startNode();

        fn();

        archive.finishNode();

    };



    //--  Binary --------//

    template<class Archive_T, typename = std::enable_if_t<!traits::is_text_archive<Archive_T>::value>>
    static void Format_As_Group(Archive_T& archive, const char* name, const std::function<void()> & fn) {

        fn();

    };


    //---------------------- Setting Array Block For Next -----------------------------------//

    //--  Write --------//

    template<typename Archive_T, typename T, typename = std::enable_if_t<traits::is_text_archive<Archive_T>::value>>
    static typename std::enable_if<cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type //why is it so hard to make this work?
    Format_As_Array(Archive_T& archive, const char* name, T & data, size_t size, bool separate_objects = false, const std::string & prefix = "") {

        if (name != nullptr && name[0] != '\0')
            archive.setNextName(name);

        archive.startNode();
        archive.makeArray();

        if (!separate_objects)
            for (size_t i = 0; i < size; i++)
                archive(data[i]);
        else
            for (size_t i = 0; i < size; i++) {
                auto val = prefix + std::to_string(i);
                archive.setNextName(val.c_str());
                archive.startNode();
                archive(data[i]);
                archive.finishNode();
            }

        archive.finishNode();

    };

    //--  Read --------//

    template<typename Archive_T, typename T, typename = std::enable_if_t<traits::is_text_archive<Archive_T>::value>>
    static typename std::enable_if<!cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type //why is it so hard to make this work?
    Format_As_Array(Archive_T& archive, const char* name, T & data, size_t size, bool separate_objects = false, const std::string & prefix = "Value") {

        if (name != nullptr && name[0] != '\0')
            archive.setNextName(name);

        archive.startNode();
        archive.makeArray();

        if (!separate_objects)
            for (size_t i = 0; i < size; i++)
                archive(data[i]);
        else
            for (size_t i = 0; i < size; i++) {
                auto val = prefix + std::to_string(i);
                archive.setNextName(val.c_str());
                archive.startNode();
                archive(data[i]);
                archive.finishNode();
            }

        archive.finishNode();

    };

    //--  Binary --------//

    template<class Archive_T, typename T, typename = std::enable_if_t<!traits::is_text_archive<Archive_T>::value>>
    static void Format_As_Array(Archive_T& archive, const char* name, T & data, size_t size, bool separate_objects = false, const std::string & prefix = "") {
        for (size_t i = 0; i < size; i++)
            archive(data[i]);

    };

    //---------------------- Directly Control Format -----------------------------------//

    namespace Control {

        template<typename Archive_T, typename = std::enable_if_t<traits::is_text_archive<Archive_T>::value>>
        static typename std::enable_if<cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type
        Start_Array(Archive_T& archive) {

            archive.makeArray();

        };

        template<typename Archive_T, typename = std::enable_if_t<traits::is_text_archive<Archive_T>::value>>
        static typename std::enable_if<cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type
        Start_Node(Archive_T& archive, const char* name = nullptr) {

            if (name != nullptr && name[0] != '\0')
                archive.setNextName(name);

            archive.startNode();
        };

        template<typename Archive_T, typename = std::enable_if_t<traits::is_text_archive<Archive_T>::value>>
        static typename std::enable_if<cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type
        Finish_Node(Archive_T& archive) {

            archive.finishNode();
        };

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

        namespace Style {

            template<typename Archive_T> static
            typename std::enable_if<cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type
            Flow(Archive_T & ar) { ar.Set_Style_Flow(); };

            template<typename Archive_T> static
            typename std::enable_if<!cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type
            Flow(Archive_T & ar) {};

            template<typename Archive_T> static
            typename std::enable_if<cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type
            Block(Archive_T & ar) { ar.Set_Style_Block(); };

            template<typename Archive_T> static
            typename std::enable_if<!cereal::traits::is_same_archive<Archive_T, YAMLOutputArchive>::value, void>::type
            Block(Archive_T & ar) {};

        }

    }

}

#endif //FORMATTING_HPP
