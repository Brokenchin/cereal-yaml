#include <cereal-yaml/archives/yaml.hpp>

#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/array.hpp>
#include <array>
#include <vector>


#include <fstream>
#include <type_traits>
#include <cereal/archives/binary.hpp>

#include "cereal-yaml/helper/formatting.hpp"

#include <cereal/archives/json.hpp>
namespace cereal {

    // template <class Archive, typename T, size_t N, std::enable_if_t<std::is_arithmetic<T>::value, int> = 1>
    // void save(Archive& archive, const std::array<T, N>& array) {
    //     //archive( make_size_tag( static_cast<size_type>(array.size()) ) );
    //     for (auto& item : array) {
    //         archive(item);
    //     }
    // }
    //
    // template <class Archive, typename T, size_t N, std::enable_if_t<std::is_arithmetic<T>::value, int> = 1>
    // void load(Archive& archive, std::array<T, N>& array) {
    //     for (auto& item : array) {
    //         archive(item);
    //     }
    // }
    //
    // //I do need templates here to only flowisize the arrays with arithmetics.
    // template <class Archive, typename T, size_t N,
    // std::enable_if_t<!std::is_arithmetic<T>::value, int> = 0>
    // void save(Archive& archive, const std::array<T, N>& array) {
    //     //archive( make_size_tag( static_cast<size_type>(array.size()) ) );
    //     //archive(cereal::binary_data(array.data() , array.size() * sizeof(T)));
    //     for (auto& item : array) {
    //         archive(item);
    //     }
    // }
    //
    // template <class Archive, typename T, size_t N,
    // std::enable_if_t<!std::is_arithmetic<T>::value, int> = 0>
    // void load(Archive& archive, std::array<T, N>& array) {
    //     // archive( make_size_tag( static_cast<size_type>(array.size()) ) );
    //     for (auto& item : array) {
    //         archive(item);
    //     }
    // }

}

struct Tag_Component {

    std::string name = "test_name";

    template <class Archive>
    void serialize(Archive& ar)
    {



        // ar.startNode();
        // ar.saveValue(5);
        ar(cereal::make_nvp("Tag", name));
        // ar.finishNode();


    }
};

struct MyRecord
{
    MyRecord() = default;
    MyRecord(uint8_t x_, uint8_t y_, float z_)
        : id(x_)
        , y(y_)
        , z(z_)
    {}

    uint8_t id, y;
    float z = 0;

    Tag_Component  tag_comp;

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(cereal::make_nvp("Entity", id));
        ar(y, cereal::make_nvp("Useless Z", z));

        ar(CEREAL_NVP(tag_comp));
    }

    friend std::ostream& operator<<(std::ostream& os, const MyRecord& myRecord);
};

inline std::ostream& operator<<(std::ostream& os, const MyRecord& myRecord)
{
    os << "{";
    os << "x=" << int(myRecord.id) << ",";
    os << "y=" << int(myRecord.y) << ",";
    os << "z=" << myRecord.z;
    os << "}";
    return os;
}

struct SomeData
{
    int32_t id;
    std::unordered_map<uint32_t, MyRecord> data;

    template <class Archive>
    void save(Archive& ar) const
    {
        ar(data);
    }

    template <class Archive>
    void load(Archive& ar)
    {
        static int32_t idGen = 0;
        id = idGen++;
        ar(data);
    }

    friend std::ostream& operator<<(std::ostream& os, const SomeData& someData);
};

inline std::ostream& operator<<(std::ostream& os, const SomeData& someData)
{
    os << "{";
    os << "id=" << someData.id << ",";
    os << "data={";
    for (auto entry : someData.data)
    {
        os << entry.first << ":" << entry.second << ",";
    }
    if (someData.data.size() > 0)
    {
        os << "\b\b";
    }
    os << "}";
    os << "}";
    return os;
}

int main()
{

    std::vector<MyRecord> arr = {MyRecord(1, 2, 3.0f), MyRecord(4, 5, 6.1f), MyRecord(7, 8, 9.2f), MyRecord(10, 11, 12.3f)};
    std::array<int, 4> arr2 = {4, 3, 2, 1};
    std::vector<int> vec = {4, 3, 2, 1};


    std::ostringstream os;
    os.clear();
    os.str("");

    std::cout << "------------ YAML -------------- :\n" << std::endl;

    {
        cereal::YAMLOutputArchive archive(os);

        //cereal::Format_As_Flow(archive, "Vector", arr);

        //cereal::Make_Named_Pair(name, data)

        // archive.setNextName("Vector");
        // archive.startNode();
        // archive.makeArray();
        //
        // for (auto& item : arr) {
        //     archive.startNode();
        //     archive(cereal::make_nvp("Ent", item.id));
        //     archive(item.y, cereal::make_nvp("Useless", item.z));
        //
        //     //archive.startNode();
        //     //archive(item.tag_comp);
        //     archive(CEREAL_NVP(item.tag_comp)); //for reason this does not require a new node.
        //     archive.finishNode();
        // }
        //
        // archive.finishNode();


        //this shows a raw way to Achieve similar format:
        cereal::Format_As_Group(archive, "Scene_Raw", [&arr, &archive]() {
            //archive.makeArray();
            for (auto& item : arr) {

                cereal::Control::Start_Node(archive, nullptr);

                cereal::Control::Start_Array(archive, "Entities");

                archive(cereal::make_nvp("Ent", item.id));
                archive(item.y, cereal::make_nvp("Useless", item.z));

                //archive.startNode();
                //archive(item.tag_comp);
                archive(CEREAL_NVP(item.tag_comp)); //for reason this does not require a new node.
                //archive.finishNode();

                cereal::Control::Finish_Node(archive);

            }

        });



        // //this works quite differently than I thought.
        cereal::Format_As_Group(archive, "Scene", [&arr, &arr2, &archive]() {
        //

            cereal::Format_As_Array(archive, "Entities", arr, 3, true, "Ent_");
            cereal::Format_As_Flow(archive, "Random_Values", arr2, 4);
            cereal::Format_As_Flow(archive, "Random_Values2", arr2, 4);
            // for (auto& item : arr) {
            //     //archive.startNode(); //this what I was missing.. I think. so cereal starts a node on each item.
            //     archive(item);
            //     //archive.finishNode();
            // }
        //
        });

        cereal::Format_As_Group(archive, "Editor", [&arr, &arr2, &archive]() {
//

            cereal::Format_As_Array(archive, "Entities", arr, 3, false, "Ent_");
            cereal::Format_As_Flow(archive, "Random_Values54", arr2, 4);
            cereal::Format_As_Flow(archive, "Random_Values56", arr2, 4);
        // for (auto& item : arr) {
        //     //archive.startNode(); //this what I was missing.. I think. so cereal starts a node on each item.
        //     archive(item);
        //     //archive.finishNode();
        // }
        //
        });


        //
        // cereal::Format_As_Flow(archive, "Array", arr2, 4);
        //
        // archive(cereal::make_nvp("Array3", vec));

    }

        std::cout << "output:\n" << os.str() << std::endl;
    //
    // {
    //     std::istringstream is(os.str());
    //
    //     std::vector<MyRecord> arr_in = {};
    //     arr_in.resize(4);
    //     std::array<int, 4> arr2_in = {};
    //     std::vector<int> vec_in = {};
    //     vec_in.resize(4);
    //
    //     cereal::YAMLInputArchive archive(is);
    //
    //
    //     //archive(cereal::make_nvp("Array", arr));
    //     cereal::Format_As_Flow(archive, "Vector", arr_in);
    //     cereal::Format_As_Flow(archive, "Array", arr2_in, 4);
    //
    //     archive(cereal::make_nvp("Array3", vec_in));
    //
    //     std::cout << "input:\n" << std::endl;
    //
    //     for (auto& item : arr_in)
    //         std::cout << item << std::endl;
    //
    //
    //     for (auto& item : arr2_in)
    //         std::cout << item << std::endl;
    //
    //     for (auto& item : vec_in)
    //         std::cout << item << std::endl;
    //
    //
    // }

    std::cout << "------------ JSON -------------- :\n" << std::endl;

    os.clear();
    os.str("");

#ifdef FALSE
    {

        cereal::JSONOutputArchive archive(os);
        cereal::Format_As_Flow(archive, "Vector", arr);
        cereal::Format_As_Flow(archive, "Array", arr2, 4);

        archive(cereal::make_nvp("Array3", vec));

    }

        std::cout << "output:\n" << os.str() << std::endl;

    {
        std::istringstream is(os.str());

        std::vector<MyRecord> arr_in = {};
        arr_in.resize(4);
        std::array<int, 4> arr2_in = {};
        std::vector<int> vec_in = {};
        vec_in.resize(4);

        cereal::JSONInputArchive archive(is);

        cereal::Format_As_Flow(archive, "Vector", arr_in);
        cereal::Format_As_Flow(archive, "Array", arr2_in, 4);

        archive(cereal::make_nvp("Array3", vec_in));

        std::cout << "input:\n" << std::endl;

        for (auto& item : arr_in)
            std::cout << item << std::endl;


        for (auto& item : arr2_in)
            std::cout << item << std::endl;

        for (auto& item : vec_in)
            std::cout << item << std::endl;


    }

    os.clear();
    os.str("");

    std::cout << "------------ BINARY -------------- :\n" << std::endl;

#endif

#ifdef FALSE
    {

        std::ofstream osb("out.cereal", std::ios::binary);

        cereal::BinaryOutputArchive archive(osb);

        cereal::Format_As_Flow(archive, "Vector", arr);
        cereal::Format_As_Flow(archive, "Array", arr2, 4);

        archive(vec);

    }

    {


        std::vector<MyRecord> arr_in = {};
        arr_in.resize(4);
        std::array<int, 4> arr2_in = {};
        std::vector<int> vec_in = {};
        vec_in.resize(4);

        std::ifstream isb("out.cereal", std::ios::binary);
        cereal::BinaryInputArchive archive(isb);

        cereal::Format_As_Flow(archive, "Vector", arr_in);
        cereal::Format_As_Flow(archive, "Array", arr2_in, 4); // this part fails.. but why. these are simply ints.. 4 in total..

        archive(vec_in);

        std::cout << "input:\n" << std::endl;

        for (auto& item : arr_in)
            std::cout << item << std::endl;

        for (auto& item : arr2_in)
            std::cout << item << std::endl;

        for (auto& item : vec_in)
            std::cout << item << std::endl;

    }

#endif


    return 0;
}
