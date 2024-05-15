#include <cereal-yaml/archives/yaml.hpp>

#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/array.hpp>
#include <fstream>

struct Tag_Component {

    std::string name;

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
        ar(cereal::make_nvp("Entity", id), y, cereal::make_nvp("Useless Z", z));
        ar(tag_comp);
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
    std::ostringstream os;
    {
        cereal::YAMLOutputArchive archive(os);
        SomeData myData;
        myData.id = 5;
        myData.data = std::unordered_map<uint32_t, MyRecord>();
        myData.data.emplace(1, MyRecord{1, 2, 3.0f});
        myData.data.emplace(2, MyRecord{4, 5, 6.1f});
        std::cout << "myData=" << myData << std::endl;
        //Beging Map Seq
        archive(cereal::make_nvp("Data" , myData));
        //Ends here.
        //begins another seq
        archive(cereal::make_nvp("Data2" , myData));
    }
    std::cout << "output=\n" << os.str() << std::endl;

    std::istringstream is(os.str());
    {
        cereal::YAMLInputArchive archive(is);
        SomeData myData;
        archive(myData);
        std::cout << "myData=" << myData << std::endl;
    }

    std::cout << "---------" << os.str() << std::endl;
    std::cout << "output2=\n" << os.str() << std::endl;

        std::ostringstream os2;
    {

        cereal::YAMLOutputArchive archive(os2);
        // bool arr[] = {true, false};
        // std::array<bool, 2> test = {true, false};
        // std::vector<MyRecord> vec = {MyRecord(1,0,0), MyRecord(2,0,0), MyRecord(3,0,0), MyRecord(4,0,0)};
        // archive(cereal::make_nvp("Scene", std::string("Untitled")),
        //         cereal::make_nvp("Entities",vec),
        //         cereal::make_nvp("arr", arr));

        archive(cereal::make_nvp("Entity", 01234566));

        //It seems to Need to implement few things in my own.
        //also a flow... how to do that.. also why array is not a flow..
        archive.setNextName("Tag Component");
        archive.startNode();
        std::array<bool, 2> test = {true, false};
        archive(cereal::make_nvp("Tag", std::string("Player")));

        // archive.setNextName("arr");
        // archive.startNode();
        archive.makeFlow("arr"); // Serializes the size

        //archive(test);
        for (auto&& value : test)
            archive(value);

        //this almost worked.. but the key is missing this outputs: [true, false] but not the array key.
        // archive.finishNode();
        // archive(cereal::make_nvp("arr", test));
        archive.finishNode();


        //Aha this will allow me to set a node in between as example for the type.
        // archive()

    }

    std::cout << "output=\n" << os2.str() << std::endl;


    return 0;
}
