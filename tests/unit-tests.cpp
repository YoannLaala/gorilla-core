#include "log/log.hpp"
#include "hash.hpp"
#include "signal.hpp"
#include "singleton.hpp"
#include "container/buffer_queue.hpp"
#include "container/command_queue.hpp"
#include "container/dictionary.hpp"
#include "container/hash_map.hpp"
#include "container/vector.hpp"
#include "file_system/file.hpp"
#include "file_system/file_system.hpp"
#include "file_system/path.hpp"
#include "memory/memory_manager.hpp"
#include "process/command_line_parser.hpp"
#include "process/process.hpp"
#include "stream/dictionary/dictionary_binary_stream.hpp"
#include "stream/dictionary/dictionary_csv_stream.hpp"
#include "stream/dictionary/dictionary_json_stream.hpp"
#include "stream/file_reader.hpp"
#include "stream/file_writer.hpp"
#include "stream/memory_reader.hpp"
#include "stream/memory_writer.hpp"
#include "string/string_helper.hpp"
#include "string/string.hpp"
#include "string/string16.hpp"
#include "thread/condition_variable.hpp"
#include "thread/mutex.hpp"
#include "thread/thread.hpp"
#include "time/profiler.hpp"
#include "time/time.hpp"

using namespace Gorilla;

#define TEST(_condition_) LOG_ASSERT(_condition_, #_condition_);

void test_hash()
{
    TEST(Hash::generate("gorilla") == 3689041125);
    TEST(Hash::generate("gorilla") == Hash::generate("gorilla", 7));
}

struct A
{
    Signal<bool, uint32_t, const char*> events;
};
struct B
{
    void on_event(bool a, uint32_t b, const char *c)
    {
        boolean = a;
        integer = b;
        string = c;
    }
    bool boolean;
    uint32_t integer;
    const char *string;
};
void test_signal()
{
    A a;
    B b;
    memset(&b, 0, sizeof(b));

    a.events.send(true, 1337, "foo");
    TEST(b.boolean == false);
    TEST(b.integer == 0);
    TEST(b.string == nullptr);

    a.events.connect(&b, &B::on_event);
    a.events.send(true, 1337, "foo");
    TEST(b.boolean == true);
    TEST(b.integer == 1337);
    TEST(strcmp(b.string, "foo") == 0);
    memset(&b, 0, sizeof(b));

    a.events.disconnect_all();
    a.events.send(true, 1337, "foo");
    TEST(b.boolean == false);
    TEST(b.integer == 0);
    TEST(b.string == nullptr);
}

class Foo {};
void test_singleton()
{
    TEST(Singleton<Foo>::is_created() == false);
    Singleton<Foo>::create();
    TEST(Singleton<Foo>::is_created() == true);
    Singleton<Foo>::destroy();
    TEST(Singleton<Foo>::is_created() == false);
}

void test_stream()
{
    MemoryWriter foo_mw;
    TEST(foo_mw.get_size() == 0);
    TEST(foo_mw.get_position() == 0);

    TEST(foo_mw.can_write());
    TEST(foo_mw.write("foobar", 6));
    TEST(foo_mw.get_size() == 6);
    TEST(foo_mw.get_position() == 6);
    TEST(foo_mw.write(1337));
    TEST(foo_mw.get_size() == 10);
    TEST(foo_mw.get_position() == 10);
    TEST(foo_mw.seek(0));
    TEST(foo_mw.get_size() == 10);
    TEST(foo_mw.get_position() == 0);

    MemoryReader foo_mr;
    TEST(foo_mr.can_read() == false);
    TEST(foo_mr.get_size() == 0);
    TEST(foo_mr.get_position() == 0);

    foo_mr.set_buffer(&foo_mw[0], foo_mw.get_size());
    TEST(foo_mr.get_size() == foo_mw.get_size());
    TEST(foo_mr.get_position() == 0);

    char foobar[6];
    TEST(foo_mr.read(foobar, sizeof(foobar)));
    TEST(MEMCMP(foobar, "foobar", sizeof(foobar)) == 0);
    TEST(foo_mr.get_size() == foo_mw.get_size());
    TEST(foo_mr.get_position() == sizeof(foobar));

    uint32_t uinteger;
    TEST(foo_mr.read(&uinteger));
    TEST(uinteger == 1337);
    TEST(foo_mr.get_size() == foo_mw.get_size());
    TEST(foo_mr.get_position() == foo_mr.get_size());

    TEST(foo_mr.seek(3));
    TEST(foo_mr.get_position() == 3);

    uint8_t character;
    TEST(foo_mr.read(&character));
    TEST(character == 'b');

    FileWriter foo_fw;
    TEST(foo_fw.can_write() == false);
    TEST(foo_fw.get_size() == 0);
    TEST(foo_fw.get_position() == 0);

    String directory, foo_filepath;
    FileSystem::get_directory_path(FileSystem::Directory::EXECUTABLE, &directory);
    Path::combine(&foo_filepath, directory, "foo");
    TEST(foo_fw.open(foo_filepath.get_buffer()) == true);
    TEST(foo_fw.can_write());
    TEST(foo_fw.write("foobar", 6));
    TEST(foo_fw.get_size() == 6);
    TEST(foo_fw.get_position() == 6);
    TEST(foo_fw.write(1337));
    TEST(foo_fw.get_size() == 10);
    TEST(foo_fw.get_position() == 10);
    TEST(foo_fw.seek(0));
    TEST(foo_fw.get_size() == 10);
    TEST(foo_fw.get_position() == 0);
    TEST(foo_fw.close());

    FileReader foo_fr;
    TEST(foo_fr.can_read() == false);
    TEST(foo_fr.get_size() == 0);
    TEST(foo_fr.get_position() == 0);

    TEST(foo_fr.open(foo_filepath.get_buffer()));
    TEST(foo_fr.can_read());
    TEST(foo_fr.get_size() == 10);
    TEST(foo_fr.get_position() == 0);

    MEMSET(foobar, 0, sizeof(foobar));
    TEST(foo_fr.read(foobar, sizeof(foobar)));
    TEST(MEMCMP(foobar, "foobar", sizeof(foobar)) == 0);
    TEST(foo_fr.get_size() == 10);
    TEST(foo_fr.get_position() == 6);

    uinteger = 0;
    TEST(foo_fr.read(&uinteger));
    TEST(uinteger == 1337);
    TEST(foo_fr.get_size() == foo_mw.get_size());
    TEST(foo_fr.get_position() == foo_mr.get_size());

    foo_fr.seek(3);
    TEST(foo_fr.get_position() == 3);

    character = 0;
    TEST(foo_fr.read(&character));
    TEST(character == 'b');
    TEST(foo_fr.close());

    TEST(FileSystem::delete_file(foo_filepath.get_buffer()) == true);
}

void test_string()
{
    String foo;
    TEST(foo.is_empty() == true);

    foo.set("foo").append("bar").append("foo");
    TEST(foo.get_length() == 9);
    TEST(foo.find("foo") == 0);
    TEST(foo.contains("bar") == true);
    TEST(foo.contains("bor") == false);
    TEST(foo.find("bar") == 3);
    TEST(foo.find_last("foo") == 6);

    foo = "";
    TEST(foo.is_empty() == true);
    TEST(foo.equals("foo") == false);

    foo.set("bar");
    TEST(foo.get_length() == 3);
    foo.prepend("foo");
    TEST(foo.get_length() == 6);
    TEST(foo.find("foo") == 0);
    foo.append("foo");
    TEST(foo.get_length() == 9);
    TEST(foo.find_last("foo") == 6);
    TEST(foo.equals("foobarfoo") == true);

    foo = "foofoo";
    TEST(foo.equals("foofoo") == true);
    foo.insert("bar", 3);
    TEST(foo.equals("foobarfoo") == true);
    foo.remove("bar");
    TEST(foo.equals("foofoo") == true);
    foo.replace("foo", "bar");
    TEST(foo.equals("barbar") == true);
    foo.to_upper();
    TEST(foo.equals("BARBAR") == true);
    foo.to_lower();
    TEST(foo.equals("barbar") == true);

    foo.set("foo").append(1337);
    TEST(foo.get_length() == 7);
    TEST(foo.equals("foo1337") == true);
    TEST(foo.equals("1337foo") == false);

    foo = "";
    TEST(foo.get_length() == 0);
    foo.resize(3);
    TEST(foo.get_length() == 3);
}

void test_string16()
{
    String16 foo;
    TEST(foo.is_empty() == true);

    foo.set(L"foo").append(L"bar").append(L"foo");
    TEST(foo.get_length() == 9);
    TEST(foo.find(L"foo") == 0);
    TEST(foo.contains(L"bar") == true);
    TEST(foo.contains(L"bor") == false);
    TEST(foo.find(L"bar") == 3);
    TEST(foo.find_last(L"foo") == 6);

    foo = L"";
    TEST(foo.is_empty() == true);
    TEST(foo.equals(L"foo") == false);

    foo.set(L"bar");
    TEST(foo.get_length() == 3);
    foo.prepend(L"foo");
    TEST(foo.get_length() == 6);
    TEST(foo.find(L"foo") == 0);
    foo.append(L"foo");
    TEST(foo.get_length() == 9);
    TEST(foo.find_last(L"foo") == 6);
    TEST(foo.equals(L"foobarfoo") == true);

    foo = L"foofoo";
    TEST(foo.equals(L"foofoo") == true);
    foo.insert(L"bar", 3);
    TEST(foo.equals(L"foobarfoo") == true);
    foo.remove(L"bar");
    TEST(foo.equals(L"foofoo") == true);
    foo.replace(L"foo", L"bar");
    TEST(foo.equals(L"barbar") == true);
    foo.to_upper();
    TEST(foo.equals(L"BARBAR") == true);
    foo.to_lower();
    TEST(foo.equals(L"barbar") == true);

    foo.set(L"foo").append(1337);
    TEST(foo.get_length() == 7);
    TEST(foo.equals(L"foo1337") == true);
    TEST(foo.equals(L"1337foo") == false);

    foo = L"";
    TEST(foo.get_length() == 0);
    foo.resize(3);
    TEST(foo.get_length() == 3);
}

void test_path()
{
    String foo;
    Path::combine(&foo, "a", "b", "c/d", "e.bin");
#if defined(PLATFORM_WINDOWS)
    TEST(foo.equals("a\\b\\c\\d\\e.bin") == true);
#else
    TEST(foo.equals("a/b/c/d/e.bin") == true);
#endif

    foo = "";
    Path::combine(&foo, "a", "b","/../b", "c/d", "../d\\e.bin");
#if defined(PLATFORM_WINDOWS)
    TEST(foo.equals("a\\b\\c\\d\\e.bin") == true);
#else
    TEST(foo.equals("a/b/c/d/e.bin") == true);
#endif

    Path bar(foo);
#if defined(PLATFORM_WINDOWS)
    TEST(bar.get_full().equals("a\\b\\c\\d\\e.bin") == true);
    TEST(bar.get_directory().equals("a\\b\\c\\d\\") == true);
#else
    TEST(bar.get_full().equals("a/b/c/d/e.bin") == true);
    TEST(bar.get_directory().equals("a/b/c/d/") == true);
#endif
    TEST(bar.get_extension().equals("bin") == true);
    TEST(bar.get_filename().equals("e") == true);
    TEST(bar.get_filename_with_extension().equals("e.bin") == true);

    foo = "";
    Path::combine(&foo, "a", "b", "c/d", "e/");

    bar = foo;
#if defined(PLATFORM_WINDOWS)
    TEST(bar.get_full().equals("a\\b\\c\\d\\e\\") == true);
    TEST(bar.get_directory().equals("a\\b\\c\\d\\e\\") == true);
#else
    TEST(bar.get_full().equals("a/b/c/d/e/") == true);
    TEST(bar.get_directory().equals("a/b/c/d/e/") == true);
#endif
    TEST(bar.get_extension().is_empty() == true);
    TEST(bar.get_filename().is_empty() == true);
    TEST(bar.get_filename_with_extension().is_empty() == true);
}

void test_string_conversion()
{
    StringValue boolean;
    StringHelper::to_value("fallse", 6, &boolean);
    TEST(boolean.DetectedType == StringValue::Type::STRING);
    StringHelper::to_value("false", 5, &boolean);
    TEST(boolean.DetectedType == StringValue::Type::BOOLEAN && boolean.Boolean == false);
    StringHelper::to_value("true", 4, &boolean);
    TEST(boolean.DetectedType == StringValue::Type::BOOLEAN && boolean.Boolean == true);
    StringHelper::to_value("tru", 3, &boolean);
    TEST(boolean.DetectedType == StringValue::Type::STRING);

    StringValue number;
    StringHelper::to_value("13.37", 5, &number);
    TEST(number.DetectedType == StringValue::Type::FLOAT && number.Float >= 13.36f && number.Float <= 13.38f);
    StringHelper::to_value("13foo37", 7, &number);
    TEST(number.DetectedType == StringValue::Type::STRING);
    StringHelper::to_value("13.foo37", 7, &number);
    TEST(number.DetectedType == StringValue::Type::STRING);
    StringHelper::to_value("37,13", 5, &number);
    TEST(number.DetectedType == StringValue::Type::FLOAT && number.Float >= 37.12 && number.Float <= 37.14);
    StringHelper::to_value("1337", 4, &number);
    TEST(number.DetectedType == StringValue::Type::INTEGER && number.Integer == 1337);
    StringHelper::to_value("-1337", 5, &number);
    TEST(number.DetectedType == StringValue::Type::INTEGER && number.Integer == -1337);
    StringHelper::to_value("0.1337f", 7, &number);
    TEST(number.DetectedType == StringValue::Type::FLOAT && number.Float >= 0.1336 && number.Float <= 0.1338);
    StringHelper::to_value("0.1337D", 7, &number);
    TEST(number.DetectedType == StringValue::Type::STRING);
    StringHelper::to_value("0.13371337d", 11, &number);
    TEST(number.DetectedType == StringValue::Type::FLOAT && number.Float >= 0.13371336 && number.Float <= 0.13371338);
    StringHelper::to_value("1337e-4", 7, &number);
    TEST(number.DetectedType == StringValue::Type::FLOAT && number.Float >= 0.1336 && number.Float <= 0.1338);
    StringHelper::to_value("1337e1", 6, &number);
    TEST(number.DetectedType == StringValue::Type::INTEGER && number.Integer == 13370);
    StringHelper::to_value("1337e+2", 7, &number);
    TEST(number.DetectedType == StringValue::Type::INTEGER && number.Integer == 133700);
    StringHelper::to_value("1337e+3d", 8, &number);
    TEST(number.DetectedType == StringValue::Type::INTEGER && number.Integer == 1337000);
    StringHelper::to_value("1337e+4f", 8, &number);
    TEST(number.DetectedType == StringValue::Type::INTEGER && number.Integer == 13370000);
    StringHelper::to_value("1337e+4F", 8, &number);
    TEST(number.DetectedType == StringValue::Type::STRING);

    String foo_number;
    TEST(StringHelper::to_string((uint64_t)18446744073709551615U, &foo_number) == true);
    TEST(foo_number == "18446744073709551615");
    TEST(StringHelper::to_string((int64_t)-9223372036854775807, &foo_number) == true);
    TEST(foo_number == "-9223372036854775807");
    TEST(StringHelper::to_string((int64_t)9223372036854775807, &foo_number) == true);
    TEST(foo_number == "9223372036854775807");
    TEST(StringHelper::to_string(-1337, &foo_number) == true);
    TEST(foo_number == "-1337");
    TEST(StringHelper::to_string((uint32_t)4294967295, &foo_number) == true);
    TEST(foo_number == "4294967295");
    TEST(StringHelper::to_string(-2147483647, &foo_number) == true);
    TEST(foo_number == "-2147483647");
    TEST(StringHelper::to_string(2147483647, &foo_number) == true);
    TEST(foo_number == "2147483647");
    TEST(StringHelper::to_string(13.37f, &foo_number) == true);
    TEST(foo_number == "13.37");
    TEST(StringHelper::to_string(13.73f, &foo_number) == true);
    TEST(foo_number == "13.73");
    TEST(StringHelper::to_string(-13.037f, &foo_number) == true);
    TEST(foo_number == "-13.037");
    TEST(StringHelper::to_string(0.001337f, &foo_number) == true);
    TEST(foo_number == "0.001337");
    TEST(StringHelper::to_string(-0.001337f, &foo_number) == true);
    TEST(foo_number == "-0.001337");

    String foobar("f.o.o.b.a.r");
    String16 foobar16;

    Vector<String> split;
    StringHelper::split(foobar.get_buffer(), ".", &split);
    TEST(split.get_size() == 6);
    TEST(split[0].equals("f"));
    TEST(split[1].equals("o"));
    TEST(split[2].equals("o"));
    TEST(split[3].equals("b"));
    TEST(split[4].equals("a"));
    TEST(split[5].equals("r"));

    String foobar2;
    StringHelper::to_string16(foobar.get_buffer(), foobar.get_length(), &foobar16);
    TEST(foobar16.equals(L"f.o.o.b.a.r"));
    StringHelper::to_string(foobar16.get_buffer(), foobar16.get_length(), &foobar2);
    TEST(foobar2.equals("f.o.o.b.a.r"));
    TEST(foobar == foobar2);
}

void test_hash_map()
{
    HashMap<int, int> values;

    values.add(1, 10);
    values.add(10, 100);
    values.add(100, 1000);
    TEST(values.get_size() == 3);
    TEST(values[1] == 10);
    TEST(values[10] == 100);
    TEST(values[100] == 1000);

    int array_results[3] = {0,0,0};
    int *results = array_results;
    HashMap<int, int>::Iterator it = values.get_first();
    HashMap<int, int>::Iterator it_end = values.get_last();
    while (it != it_end) {
        *results++ = it.get_value();
        ++it;
    }
    TEST(array_results[0] + array_results[1] + array_results[2] == 1110);

    values.clear();
    TEST(values.is_empty() == true);
}

void test_vector()
{
    Vector<int> values;

    TEST(values.is_empty() == true);
    TEST(values.get_size() == 0)
    TEST(values.get_capacity() == 0);

    values.add(18);
    TEST(values.is_empty() == false);
    TEST(values.get_size() == 1)
    TEST(values.get_capacity() > 1);
    TEST(values[0] == 18);

    values.resize(8, 1337);
    TEST(values.get_size() == 8)
    TEST(values.get_capacity() >= 8);
    TEST(values[0] == 1337);

    values.insert(2, 1338);
    TEST(values.get_size() == 9)
    TEST(values[2] == 1338);

    TEST(values.find_index(1338) == 2)
    values.remove_index(2);
    TEST(values.find_index(1338) == (uint32_t)-1);

    values.clear();
    TEST(values.is_empty() == true);
    TEST(values.get_capacity() > 0);

    values.expand(1);
    TEST(values.get_size() == 1);

    values.expand_to(128, 88);
    TEST(values.get_size() == 129)
    TEST(values.get(88) == 88);

    values.clear();
    values.add(3); values.add(2); values.add(1); values.add(0);
    for (uint32_t i = 0; i < 4; ++i)
        TEST(values[i] == 3-i);
    values.sort();
    for (uint32_t i = 0; i < 4; ++i)
        TEST(values[i] == i);

    int stack_array[4] = { 1, 3, 3, 7 };
    values.set_buffer(stack_array, 2, 4, false);
    TEST(values.get_size() == 2);
    TEST(values.get_capacity() == 4);
    TEST(values[1] == 3);

    values.remove(7);
    TEST(values.get_size() == 2);
    TEST(values.find_index(3) == 1);
    values.remove(3);
    TEST(values.find_index(3) == (uint32_t)-1);
    values.remove(3);
    TEST(values.get_size() == 1);
    TEST(values.get_capacity() == 4);
    TEST(values[0] == 1);
}

void test_buffer_queue()
{
    BufferQueue queue;

    TEST(queue.is_empty() == true);
    TEST(queue.get_size() == 0)
    TEST(queue.get_capacity() == 0);
    TEST(queue.pop() == nullptr);

    uint8_t data1[] = { 0, 1, 2, 3};
    uint64_t data2[] = { 0, 1, 2, 3};
    float data3[] = { 0.5f, 1.5f, 2.5f, 3.5f};

    queue.push(data1, sizeof(data1));
    queue.push(data2, sizeof(data2));
    queue.push(data3, sizeof(data3));

    uint8_t *data4 = (uint8_t*)queue.push(4);
    data4[0] = 4;
    data4[1] = 5;
    data4[2] = 6;
    data4[3] = 7;

    TEST(queue.is_empty() == false);
    TEST(queue.get_size() == 72)
    TEST(queue.get_capacity() >= queue.get_size());
    uint32_t max_size = queue.get_size();
    uint32_t max_capacity = queue.get_capacity();

    TEST(MEMCMP(queue.pop(), data1, sizeof(data1)) == 0);
    TEST(MEMCMP(queue.pop(), data2, sizeof(data2)) == 0);
    TEST(MEMCMP(queue.pop(), data3, sizeof(data3)) == 0);
    TEST(MEMCMP(queue.pop(), data4, 4*sizeof(uint8_t)) == 0);

    TEST(queue.get_size() == 0)
    TEST(queue.get_capacity() >= queue.get_size());
    queue.clear();
    TEST(queue.get_size() == 0);
    TEST(queue.pop() == nullptr);

    queue.push(data1, sizeof(data1));
    queue.push(data2, sizeof(data2));
    TEST(MEMCMP(queue.pop(), data1, sizeof(data1)) == 0);
    queue.push(data3, sizeof(data3));
    TEST(MEMCMP(queue.pop(), data2, sizeof(data2)) == 0);
    TEST(MEMCMP(queue.pop(), data3, sizeof(data3)) == 0);
    TEST(queue.pop() == nullptr);

    queue.push(data1, sizeof(data1));
    queue.push(data2, sizeof(data2));
    queue.push(data3, sizeof(data3));
    queue.push(4);
    TEST(queue.get_size() == max_size)
    TEST(queue.get_capacity() == max_capacity)

    TEST(MEMCMP(queue.pop(), data1, sizeof(data1)) == 0);
    TEST(MEMCMP(queue.pop(), data2, sizeof(data2)) == 0);
    TEST(MEMCMP(queue.pop(), data3, sizeof(data3)) == 0);
    TEST(MEMCMP(queue.pop(), data4, 4*sizeof(uint8_t)) == 0);
    TEST(queue.get_size() == 0)
    TEST(queue.get_capacity() == max_capacity);

    BufferQueue queue2;
    queue2.swap(queue);

    TEST(queue.is_empty() == true);
    TEST(queue.get_size() == 0)
    TEST(queue.get_capacity() == 0);

    queue2.push(data1, sizeof(data1));
    queue2.push(data2, sizeof(data2));
    queue2.push(data3, sizeof(data3));
    queue2.push(4);
    TEST(queue2.get_size() == max_size)
    TEST(queue2.get_capacity() == max_capacity)

    TEST(MEMCMP(queue2.pop(), data1, sizeof(data1)) == 0);
    TEST(MEMCMP(queue2.pop(), data2, sizeof(data2)) == 0);
    TEST(MEMCMP(queue2.pop(), data3, sizeof(data3)) == 0);
    TEST(MEMCMP(queue2.pop(), data4, 4*sizeof(uint8_t)) == 0);
    TEST(queue2.get_size() == 0)
    TEST(queue2.get_capacity() == max_capacity);
}

int command_queue_callback_0()
{
    return -1;
}

int command_queue_callback_1(void *data)
{
    **(int**)(data) = 3;
    return 1;
}

int command_queue_callback_2(void *data)
{
    **(int**)(data) = 2;
    return 2;
}

struct CommandQueueStruct
{
    int command_queue_callback_3()
    {
        return 3;
    }

    int command_queue_callback_4(void *data)
    {
        **(int**)(data) = 1;
        return 4;
    }
};

struct CommandData
{
    String foo;
};

int command_queue_callback_5(void *data)
{
    CommandData *foo = (CommandData*)data;
    if (foo->foo == "bar")
        return 0;
    return -1;
}

void test_command_queue()
{
    CommandQueueStruct object;
    CommandQueue queue;

    TEST(queue.is_empty() == true);
    TEST(queue.get_size() == 0)
    TEST(queue.get_capacity() == 0);
    TEST(queue.pop() == 0);

    int value = 0;
    queue.push(&command_queue_callback_0);
    TEST(queue.pop() == -1);
    TEST(queue.pop() == 0);

    queue.push(&command_queue_callback_1, &value);
    queue.push(&command_queue_callback_2, &value);
    queue.push<CommandQueueStruct, &CommandQueueStruct::command_queue_callback_3>(&object);
    queue.push<CommandQueueStruct, &CommandQueueStruct::command_queue_callback_4>(&object, &value);

    TEST(queue.is_empty() == false);
    TEST(queue.get_capacity() >= queue.get_size());

    TEST(value == 0);
    TEST(queue.pop() == 1);
    TEST(value == 3);
    TEST(queue.pop() == 2);
    TEST(value == 2);
    TEST(queue.pop() == 3);
    TEST(value == 2);
    TEST(queue.pop() == 4);
    TEST(value == 1);

    TEST(queue.is_empty() == true);
    TEST(queue.get_size() == 0);


    CommandData command_data =  {
        "bar"
    };
    CommandQueue foo_queue;
    foo_queue.push(&command_queue_callback_5, command_data);
    TEST(foo_queue.pop() == 0);
    foo_queue.push(&command_queue_callback_5, std::move(command_data));
    TEST(foo_queue.pop() == 0);
}

void test_dictionary_json(Dictionary *dictionary)
{
    DictionaryNode foo = (*dictionary)["hello"];
    TEST(foo.is_string() && STRCMP(foo.get_string(), "world") == 0);

    foo = (*dictionary)["t"];
    TEST(foo.is_bool() && foo.get_bool() == true);

    foo = (*dictionary)["f"];
    TEST(foo.is_bool() && foo.get_bool() == false);

    foo = (*dictionary)["n"];
    TEST(foo.is_null() == true);

    foo = (*dictionary)["i"];
    TEST(foo.is_int64() == true && foo.get_int64() == 123);

    foo = (*dictionary)["pi"];
    TEST(foo.is_float64() == true && foo.get_float64() > 3.1415 && foo.get_float64() < 3.1417);

    foo = (*dictionary)["ai"];
    TEST(foo.is_buffer() == true && foo.get_buffer_format() == DictionaryData::Format::INTEGER64);

    uint32_t int64_count;
    const int64_t *int64_buffer;
    foo.get_buffer_int64(&int64_buffer, &int64_count);
    TEST(int64_count == 4);
    for (uint32_t i = 0; i < int64_count; ++i)
        TEST(int64_buffer[i] == (int64_t)i+1);

    foo = (*dictionary)["ab"];
    TEST(foo.is_buffer() == true && foo.get_buffer_format() == DictionaryData::Format::BOOLEAN);

    uint32_t bool_count;
    const bool *bool_buffer;
    foo.get_buffer_bool(&bool_buffer, &bool_count);
    TEST(bool_count == 4);
    for (uint32_t i = 0; i < bool_count; ++i)
        TEST(bool_buffer[i] == (bool)!(i%2));

    foo = (*dictionary)["af"];
    TEST(foo.is_buffer() == true && foo.get_buffer_format() == DictionaryData::Format::FLOAT64);

    uint32_t float64_count;
    const double *float64_buffer;
    foo.get_buffer_float64(&float64_buffer, &float64_count);
    TEST(float64_count == 4);
    for (uint32_t i = 0; i < float64_count; ++i)
        TEST(float64_buffer[i] == (double)i+1);

    foo = (*dictionary)["ao"];
    TEST(foo.is_array() == true);
    foo = foo.get_first_child();
    TEST(foo.is_object() == true);
    foo = foo["ab_sub"];
    TEST(foo.is_buffer() == true && foo.get_buffer_format() == DictionaryData::Format::BOOLEAN);
    foo.get_buffer_bool(&bool_buffer, &bool_count);
    TEST(bool_count == 5);
    for (uint32_t i = 0; i < bool_count; ++i)
        TEST(bool_buffer[i] == (bool)(i%2));

    foo = (*dictionary)["as"];
    TEST(foo.is_array() == false);
    TEST(foo.is_buffer() == true && foo.get_buffer_format() == DictionaryData::Format::STRING);
    Vector<const char*> foo_strings;
    foo.get_buffer_string(&foo_strings);
    TEST(foo_strings.get_size() == 4);
    for (uint32_t i = 0; i < foo_strings.get_size(); ++i)
        TEST(*foo_strings[i] == '1' + i);

    foo = (*dictionary)["ae"];
    TEST(foo.is_array() == true);
    TEST(foo.get_first_child() == DictionaryNode::INVALID);

    foo = (*dictionary)["aoe"];
    TEST(foo.is_object() == true);
    TEST(foo.get_first_child() == DictionaryNode::INVALID);
}

void test_dictionary_csv(Dictionary *dictionary)
{
    DictionaryNode foo = (*dictionary)["Year"];
    TEST(foo.is_valid() && foo.is_buffer() == true);

    uint32_t int64_count;
    const int64_t *int64_buffer;
    foo.get_buffer_int64(&int64_buffer, &int64_count);
    TEST(int64_count == 4);
    TEST(int64_buffer[0] == 1997 && int64_buffer[1] == 1999 && int64_buffer[2] == 1999 && int64_buffer[3] == 1996);

    Vector<const char*> foo_strings;
    foo = (*dictionary)["Make"];
    TEST(foo.is_valid() && foo.is_buffer() == true && foo.get_buffer_format() == DictionaryData::Format::STRING);
    foo.get_buffer_string(&foo_strings);
    TEST(foo_strings.get_size() == 4);
    TEST(STRCMP(foo_strings[3], "Jeep") == 0);

    foo = (*dictionary)["Model"];
    TEST(foo.is_valid() && foo.is_buffer() == true && foo.get_buffer_format() == DictionaryData::Format::STRING);
    foo.get_buffer_string(&foo_strings);
    TEST(foo_strings.get_size() == 4);
    TEST(STRCMP(foo_strings[0], "E350") == 0);

    foo = (*dictionary)["Description"];
    TEST(foo.is_valid() && foo.is_buffer() == true && foo.get_buffer_format() == DictionaryData::Format::STRING);
    foo.get_buffer_string(&foo_strings);
    TEST(foo_strings.get_size() == 4);
    TEST(STRCMP(foo_strings[3], "MUST SELL!\n        air; moon roof; loaded") == 0);

    foo = (*dictionary)["Price"];
    TEST(foo.is_valid() && foo.is_buffer() == true);

    uint32_t float64_count;
    const double *float64_buffer;
    foo.get_buffer_float64(&float64_buffer, &float64_count);
    TEST(float64_count == 4);
    TEST(float64_buffer[0] == 3000.0 && float64_buffer[1] == 4900.0 && float64_buffer[2] == 5000.0 && float64_buffer[3] == 4799.0);
}

void test_dictionary()
{
    Dictionary dictionary;
    TEST(dictionary.is_object() == true);
    DictionaryNode foo = dictionary.add("foo");
    TEST(foo.is_object() == true);
    foo.set_bool(false);
    TEST(foo.is_object() == false);
    TEST(foo.is_string() == false);
    TEST(foo.is_bool() == true);
    TEST(foo.get_bool() == false);
    foo.set_bool(true);
    TEST(foo.is_object() == false);
    TEST(foo.is_string() == false);
    TEST(foo.is_bool() == true);
    TEST(foo.get_bool() == true);
    foo.set_string("bar", 3);
    TEST(foo.is_object() == false);
    TEST(foo.is_bool() == false);
    TEST(foo.is_string() == true);
    TEST(STRCMP("bar", foo.get_string()) == 0);
    foo.set_string("foobar", 6);
    TEST(foo.is_bool() == false);
    TEST(foo.is_string() == true);
    TEST(STRCMP("foobar", foo.get_string()) == 0);
    dictionary["foo"] = true;
    TEST(foo.is_bool() == true);
    TEST(foo.is_string() == false);
    TEST(foo.get_bool() == true);
    TEST(STRCMP(foo.get_name(), "foo") == 0);
    foo.set_array();
    TEST(foo.is_array() == true);
    foo.add() = 0;
    foo.add() = 1;
    foo.add() = 2;
    TEST(foo.is_array() == true);
    TEST(STRCMP(foo.get_name(), "foo") == 0);

    int32_t index = 0;
    DictionaryNode child = foo.get_first_child();
    while (child != DictionaryNode::INVALID)
    {
        TEST(child.get_name() == nullptr);
        TEST(child.get_int32() == index);
        bool result = child.next();
        ++index;

        TEST(result == true && index < 3 || result == false && index == 3);
    }

    dictionary.clear();
    TEST(dictionary.find("foo") == DictionaryNode::INVALID);
    TEST(dictionary.is_object() == true);
    dictionary.add();
    TEST(dictionary.is_array() == true);
    dictionary.set_object();
    TEST(dictionary.is_object() == true);

    dictionary.clear();
    DictionaryNode bar = dictionary.add("bar");
    DictionaryNode foobar = dictionary.add("foobar");
    TEST(dictionary.find("bar") == bar && bar.is_valid());
    TEST(dictionary.find("foobar") == foobar && foobar.is_valid());
    dictionary.remove("bar");
    TEST(dictionary.find("bar").is_valid() == false);
    TEST(dictionary.find("foobar").is_valid() == true);

    dictionary.clear();
    bool foo_bool_buffer[6] { false, true, false, true, false, true };
    DictionaryNode foo_bool = dictionary.add("foo_buffer_bool");
    TEST(foo_bool.is_object() == true);
    foo_bool.set_buffer_bool(foo_bool_buffer, sizeof(foo_bool_buffer) / sizeof(*foo_bool_buffer));
    TEST(foo_bool.is_object() == false);
    TEST(foo_bool.is_buffer() == true);
    TEST(foo_bool.get_buffer_format() == DictionaryData::Format::BOOLEAN);

    uint32_t bool_count;
    const bool *bool_buffer;
    foo_bool.get_buffer_bool(&bool_buffer, &bool_count);
    TEST(MEMCMP(foo_bool_buffer, bool_buffer, bool_count * sizeof(*foo_bool_buffer)) == 0);

    int32_t foo_int32_buffer[6] { 0, 1, 2, 3, 4, 5 };
    DictionaryNode foo_int32 = dictionary.add("foo_buffer_int32");
    TEST(foo_int32.is_object() == true);
    foo_int32.set_buffer_int32(foo_int32_buffer, sizeof(foo_int32_buffer) / sizeof(*foo_int32_buffer));
    TEST(foo_int32.is_object() == false);
    TEST(foo_int32.is_buffer() == true);
    TEST(foo_int32.get_buffer_format() == DictionaryData::Format::INTEGER32);

    uint32_t int32_count;
    const int32_t *int32_buffer;
    foo_int32.get_buffer_int32(&int32_buffer, &int32_count);
    TEST(MEMCMP(foo_int32_buffer, int32_buffer, int32_count * sizeof(*foo_int32_buffer)) == 0);

    const char* foo_string_buffer[6] { "foo0", "bar0", "foo1", "bar1", "foo2", "bar2" };
    DictionaryNode foo_strings = dictionary.add("foo_buffer_string");
    TEST(foo_strings.is_object() == true);
    foo_strings.set_buffer_string(foo_string_buffer, sizeof(foo_string_buffer) / sizeof(*foo_string_buffer));
    TEST(foo_strings.is_object() == false);
    TEST(foo_strings.is_buffer() == true);
    TEST(foo_strings.get_buffer_format() == DictionaryData::Format::STRING);
    TEST(foo_strings.is_array() == false);

    Vector<const char*> foo_string_values;
    foo_strings.get_buffer_string(&foo_string_values);
    TEST(foo_string_values.get_size() == sizeof(foo_string_buffer) / sizeof(*foo_string_buffer));
    for (uint32_t i = 0; i < sizeof(foo_string_buffer) / sizeof(*foo_string_buffer); ++i)
        TEST(STRCMP(foo_string_values[i], foo_string_buffer[i]) == 0);

    char json[] = "{\
        \"hello\": \"world\",\
        \"t\": true ,\
        \"f\": false,\
        \"n\": null,\
        \"i\": 123,\
        \"pi\": 3.1416,\
        \"ai\": [1, 2, 3, 4],\
        \"ab\": [true, false, true, false],\
        \"af\": [1.0, 2.0, 3.0, 4.0],\
        \"ao\": [{\"ab_sub\":[false, true, false, true, false]}],\
        \"as\": [\"1\", \"2\", \"3\", \"4\"],\
        \"ae\": [],\
        \"aoe\": {}\
    }";
    MemoryReader reader(json, sizeof(json));
    TEST(dictionary.read<DictionaryJsonStream>(&reader) == true);
    test_dictionary_json(&dictionary);

    String foo_filepath;
    FileSystem::get_directory_path(FileSystem::Directory::EXECUTABLE, &foo_filepath);
    Path::combine(&foo_filepath, "foo");
    TEST(dictionary.write<DictionaryJsonStream>(foo_filepath.get_buffer(), (uint32_t)DictionaryJsonStream::Flag::PRETTY));

    Dictionary dictionary_json;
    TEST(dictionary_json.read<DictionaryJsonStream>(foo_filepath.get_buffer()));
    test_dictionary_json(&dictionary_json);
    TEST(dictionary_json.write<DictionaryBinaryStream>(foo_filepath.get_buffer()));

    Dictionary dictionary_binary;
    TEST(dictionary_binary.read<DictionaryBinaryStream>(foo_filepath.get_buffer()));
    test_dictionary_json(&dictionary_binary);
    TEST(dictionary_binary.write<DictionaryBinaryStream>(foo_filepath.get_buffer()));

    char csv[] = "\
        Year;Make;Model;Description;Price\n\
        1997;Ford;E350;\"ac; abs; moon\";3000.00\n\
        1999;Chevy;\"Venture \"\"Extended Edition\"\"\";\"\";4900.00\n\
        1999;Chevy;\"Venture \"\"Extended Edition; Very Large\"\"\";;5000.00\n\
        1996;Jeep;Grand Cherokee;\"MUST SELL!\n\
        air; moon roof; loaded\";4799.00";
    reader.set_buffer(csv, sizeof(csv));
    Dictionary dictionary_csv;
    TEST(dictionary_csv.read<DictionaryCsvStream>(&reader));
    test_dictionary_csv(&dictionary_csv);
    TEST(dictionary_csv.write<DictionaryCsvStream>(foo_filepath.get_buffer()));
    TEST(dictionary.read<DictionaryCsvStream>(foo_filepath.get_buffer()));
    test_dictionary_csv(&dictionary);
    TEST(FileSystem::delete_file(foo_filepath.get_buffer()) == true);
}

void test_file_system()
{
    String directory;
    FileSystem::get_directory_path(FileSystem::Directory::EXECUTABLE, &directory);
    TEST(directory.get_length() > 0);
    TEST(FileSystem::check_directory(directory.get_buffer()) == true);
    TEST(FileSystem::check_file(directory.get_buffer()) == false);

    String foo_filepath;
    Path::combine(&foo_filepath, directory, "foo");
    char content[] = { 'f', 'o', 'o' };

    File *file = FileSystem::open_file(foo_filepath.get_buffer(), FileSystem::Access::WRITE);
    TEST(file != nullptr);
    TEST(file->write(content, sizeof(content)) == sizeof(content));
    TEST(file->get_size() == sizeof(content));
    TEST(file->get_position() == sizeof(content));
    file->seek(0);
    TEST(file->get_position() == 0);

    // can't read with write mode so 0 expected
    char *buffer = new char[file->get_size()];
    TEST(file->read(buffer, file->get_size()) == 0);
    TEST(file->get_position() == 0);
    TEST(FileSystem::close_file(file) == true);
    delete[] buffer;

    TEST(FileSystem::check_directory(foo_filepath.get_buffer()) == false);
    TEST(FileSystem::check_file(foo_filepath.get_buffer()) == true);

    String bar_filepath;
    Path::combine(&bar_filepath, directory, "bar");
    TEST(FileSystem::copy_file(bar_filepath.get_buffer(), foo_filepath.get_buffer()) == false);
    TEST(FileSystem::copy_file(foo_filepath.get_buffer(), bar_filepath.get_buffer()) == true);
    TEST(FileSystem::delete_file(foo_filepath.get_buffer()) == true);
    TEST(FileSystem::check_directory(foo_filepath.get_buffer()) == false);
    TEST(FileSystem::check_file(foo_filepath.get_buffer()) == false);
    TEST(FileSystem::check_directory(bar_filepath.get_buffer()) == false);
    TEST(FileSystem::check_file(bar_filepath.get_buffer()) == true);
    TEST(FileSystem::move(foo_filepath.get_buffer(), bar_filepath.get_buffer()) == false);
    TEST(FileSystem::move(bar_filepath.get_buffer(), foo_filepath.get_buffer()) == true);

    file = FileSystem::open_file(foo_filepath.get_buffer(), FileSystem::Access::READ);
    TEST(file != nullptr);
    TEST(file->get_size() == sizeof(content));
    TEST(file->get_position() == 0);

    buffer = new char[file->get_size()];
    TEST(file->read(buffer, file->get_size()) == sizeof(content));
    TEST(file->get_position() == file->get_size());
    TEST(MEMCMP(content, buffer, file->get_size()) == 0);
    TEST(file->write(content, sizeof(content)) == 0);
    TEST(FileSystem::close_file(file) == true);
    delete[] buffer;

    TEST(FileSystem::check_directory(bar_filepath.get_buffer()) == false);
    TEST(FileSystem::create_directory(bar_filepath.get_buffer()) == true);
    TEST(FileSystem::check_directory(bar_filepath.get_buffer()) == true);
    TEST(FileSystem::delete_directory(bar_filepath.get_buffer()) == true);
    TEST(FileSystem::check_directory(bar_filepath.get_buffer()) == false);
    TEST(FileSystem::create_directory(bar_filepath.get_buffer()) == true);

    String foobar_filepath;
    Path::combine(&foobar_filepath, directory, "foo.bar");
    TEST(FileSystem::check_file(foobar_filepath.get_buffer()) == false);
    file = FileSystem::open_file(foobar_filepath.get_buffer(), FileSystem::Access::WRITE);
    TEST(file != nullptr);
    TEST(FileSystem::close_file(file) == true);
    TEST(FileSystem::check_file(foobar_filepath.get_buffer()) == true);

    Vector<String> directories, files;
    TEST(FileSystem::list(foobar_filepath.get_buffer(), nullptr, nullptr) == false);
    TEST(FileSystem::list(directory.get_buffer(), nullptr, nullptr) == true);
    TEST(FileSystem::list(directory.get_buffer(), &directories, &files) == true);

    bool found = false;
    for (uint32_t i = 0; i < directories.get_size(); ++i) {
        if (directories[i].contains(bar_filepath.get_buffer())) {
            found = true;
            break;
        }
    }
    TEST(found);

    for (uint32_t i = 0; i < files.get_size(); ++i) {
        if (files[i] == foo_filepath) {
            found = true;
            break;
        }
    }
    TEST(found);
    TEST(FileSystem::list(directory.get_buffer(), &directories, &files, ".bar") == true);
    TEST(directories.get_size() > 0);
    TEST(files.get_size() == 1)
    TEST(files[0] == foobar_filepath);
    TEST(FileSystem::list(directory.get_buffer(), &directories, &files, ".bar;.exe;.a") == true);
    TEST(directories.get_size() > 0);
    TEST(files.get_size() > 1);

    String random_directory;
    Path::combine(&random_directory, directory, Time::get_performance_counter(), PATH_SEPARATOR);
    TEST(FileSystem::create_directory(random_directory.get_buffer()) == true);
    TEST(FileSystem::check_directory(random_directory.get_buffer()) == true);
    TEST(FileSystem::check_file(random_directory.get_buffer()) == false);
    TEST(FileSystem::delete_directory(random_directory.get_buffer()) == true);
    TEST(FileSystem::check_directory(random_directory.get_buffer()) == false);

    TEST(FileSystem::delete_file(foo_filepath.get_buffer()) == true);
    TEST(FileSystem::check_file(foo_filepath.get_buffer()) == false);
    TEST(FileSystem::delete_directory(bar_filepath.get_buffer()) == true);
    TEST(FileSystem::check_directory(bar_filepath.get_buffer()) == false);
    TEST(FileSystem::delete_file(foobar_filepath.get_buffer()) == true);
    TEST(FileSystem::check_file(foobar_filepath.get_buffer()) == false);
}

void test_memory()
{
    enum MemoryDomain
    {
        DEFAULT = 0,
        TEST0,
        TEST1,
        TEST2,

        COUNT,
    };

    MemoryStatistics statistics;
    MemoryManager::get_instance()->get_statistics(&statistics);
    TEST(statistics.memory_allocated == 0);

    static uint64_t alllocation_header_size = 16;
    static struct {
        void *allocation;
        uint32_t domain;
        uint64_t size;
    } allocation_descs[] = {
        {nullptr, DEFAULT, 16},
        {nullptr, TEST0, 512},
        {nullptr, TEST1, 2*1024},
        {nullptr, TEST2, 5*1024*1024},
        {nullptr, DEFAULT, 8},
        {nullptr, DEFAULT, 64},
        {nullptr, DEFAULT, 32},
        {nullptr, DEFAULT, 16},
        {nullptr, DEFAULT, 8},
        {nullptr, DEFAULT, 32},
        {nullptr, DEFAULT, 128},
    };
    const uint32_t allocation_count = sizeof(allocation_descs) / sizeof(allocation_descs[0]);

    for (int i = 0; i < allocation_count; ++i) {
        MemoryManager::get_instance()->set_domain(allocation_descs[i].domain);
        allocation_descs[i].allocation = MemoryManager::get_instance()->allocate(allocation_descs[i].size);
    }

    static const uint64_t domain_sizes[COUNT] = {
        alllocation_header_size * 8 + 304,
        alllocation_header_size + 512,
        alllocation_header_size + 2*1024,
        alllocation_header_size + 5*1024*1024,
    };

    MemoryManager::get_instance()->get_statistics(&statistics);

    for (int i = 0; i < COUNT; ++i)
        TEST(statistics.domains_allocated[i] == domain_sizes[i]);

    for (int i = 0; i < allocation_count; ++i)
        MemoryManager::get_instance()->deallocate(allocation_descs[i].allocation);

    MemoryManager::get_instance()->get_statistics(&statistics);
    TEST(statistics.memory_allocated == 0);

    void *a = MemoryManager::get_instance()->allocate(8);
    void *b = MemoryManager::get_instance()->allocate(8);
    void *c = MemoryManager::get_instance()->allocate(8);

    MemoryManager::get_instance()->get_statistics(&statistics);
    TEST(statistics.page_allocated == 1);
    TEST(statistics.memory_allocated > statistics.memory_used);
    TEST(statistics.allocation_used > 1);

    MemoryManager::get_instance()->deallocate(a);
    MemoryManager::get_instance()->deallocate(b);
    MemoryManager::get_instance()->deallocate(c);

    MemoryManager::get_instance()->get_statistics(&statistics);
    TEST(statistics.memory_allocated == 0);
}

struct FooBarProduct
{
    int32_t value;
    bool foo_done;
    int64_t foo_end_time;
    int64_t bar_end_time;
    ConditionVariable foo_condition_variable;
    ConditionVariable bar_condition_variable;
};

bool foo_thread_callback_predicate(void *data)
{
    FooBarProduct *product = (FooBarProduct*)data;
    return product->value % 2 == 0;
}

void foo_thread_callback(void *data)
{
    FooBarProduct *product = (FooBarProduct*)data;

    product->foo_done = false;
    while(product->value < 100)
    {
        if (product->value % 2 == 0)
        {
            ++product->value;
            product->bar_condition_variable.signal();
        }

        product->foo_condition_variable.wait(foo_thread_callback_predicate, product);
    }
    product->foo_end_time = Time::get_performance_counter();
    product->foo_done = true;
    product->bar_condition_variable.signal();
}

bool bar_thread_callback_predicate(void *data)
{
    FooBarProduct *product = (FooBarProduct*)data;
    return product->foo_done || product->value % 2 == 1;
}

void bar_thread_callback(void *data)
{
    FooBarProduct *product = (FooBarProduct*)data;

    while(product->value < 100 || !product->foo_done)
    {
        if (product->value % 2 == 1)
        {
            ++product->value;
            product->foo_condition_variable.signal();
        }

        product->bar_condition_variable.wait(bar_thread_callback_predicate, product);
    }
    product->bar_end_time = Time::get_performance_counter();
}

void test_thread()
{
    FooBarProduct product;
    product.value = 0;

    Thread foo_thread("foo", foo_thread_callback, &product);
    Thread bar_thread("bar", bar_thread_callback, &product);
    TEST(foo_thread.get_id() != bar_thread.get_id());
    foo_thread.join();
    bar_thread.join();
    TEST(product.value == 100);
    TEST(product.foo_end_time < product.bar_end_time);

    int64_t start = Time::get_performance_counter();
    Thread::sleep_ns((uint64_t)(16*1e6));
    float elapsed_time = Time::get_elapsed_time(start, Time::get_performance_counter());
    TEST(elapsed_time >= 15.f);
}

void test_process()
{
    CommandLineParser parser("foo", "foobar", "1.0.0");
    parser.add_option("bool", "bool option");
    parser.add_option("integer", "integer option");
    parser.add_option("float", "float option");
    parser.add_option("string", "string option");
    parser.add_option("trigger", "trigger option", true);

    const char* argument_00 [] = { "bool", "float", "trigger" };
    parser.parse(argument_00, sizeof(argument_00) / sizeof(*argument_00));
    TEST(parser.has_bool("bool") == false);
    TEST(parser.has_integer("integer") == false);
    TEST(parser.has_float("float") == false);
    TEST(parser.has_string("string") == false);
    TEST(parser.has_trigger("trigger") == false);

    const char* argument_01 [] = { "--bool", "--float", "--trigger" };
    parser.parse(argument_01, sizeof(argument_01) / sizeof(*argument_01));
    TEST(parser.has_bool("bool") == false);
    TEST(parser.has_integer("integer") == false);
    TEST(parser.has_float("float") == false);
    TEST(parser.has_string("string") == false);
    TEST(parser.has_trigger("trigger") == true);

    const char* argument_02 [] = { "--bool", "13.37f", "--integer", "1337", "--string", "13.37f", "--float", "true", "-trigger" };
    parser.parse(argument_02, sizeof(argument_02) / sizeof(*argument_02));
    TEST(parser.has_bool("bool") == false);
    TEST(parser.has_integer("integer") == true);
    TEST(parser.get_integer("integer") == 1337);
    TEST(parser.has_float("float") == false);
    TEST(parser.has_string("string") == false);
    TEST(parser.has_trigger("trigger") == false);

    const char* argument_03 [] = { "--bool", "true", "--integer", "1337", "--float", "13.37f", "--string", "foo", "--trigger" };
    parser.parse(argument_03, sizeof(argument_02) / sizeof(*argument_02));
    TEST(parser.has_bool("bool") == true);
    TEST(parser.get_bool("bool") == true);
    TEST(parser.has_integer("integer") == true);
    TEST(parser.get_integer("integer") == 1337);
    TEST(parser.has_float("float") == true);
    TEST(parser.get_float("float") >= 13.36f && parser.get_float("float") <= 13.38f);
    TEST(parser.has_string("string") == true);
    TEST(STRCMP(parser.get_string("string"), "foo") == 0);
    TEST(parser.has_trigger("trigger") == true);

    Process process_foo("foo");
    TEST(process_foo.execute() == -1);
#if defined(PLATFORM_WINDOWS)
    Process process_cmd("cmd");
#else
    Process process_cmd("/bin/ls");
#endif
    TEST(process_cmd.execute() == 0);
#if defined(PLATFORM_WINDOWS)
    TEST(process_cmd.execute("/c", "dir") == 0);
#else
    TEST(process_cmd.execute("-lia") == 0);
#endif
    TEST(process_cmd.get_stdout().get_length() > 0);
}

int32_t main(int32_t argc, const char *argv[])
{
    static const struct {
        const char *name;
        void (*callback)();
    } tests[] = {
        {"hash",                &test_hash},
        {"singleton",           &test_singleton},
        {"vector",              &test_vector},
        {"buffer_queue",        &test_buffer_queue},
        {"command_queue",       &test_command_queue},
        {"signal",              &test_signal},
        {"hash_map",            &test_hash_map},
        {"memory",              &test_memory},
        {"string",              &test_string},
        {"string16",            &test_string16},
        {"string_conversion",   &test_string_conversion},
        {"stream",              &test_stream},
        {"path",                &test_path},
        {"file_system",         &test_file_system},
        {"thread",              &test_thread},
        {"dictionary",          &test_dictionary},
        {"process",             &test_process},
    };

    const char *unit_test_name = argc < 2 ? nullptr: argv[1];
    const uint32_t test_count = sizeof(tests) / sizeof(*tests);
    for(uint32_t i = 0; i < test_count; i++) {
        bool execute_test = unit_test_name == nullptr || strcmp(tests[i].name, unit_test_name) == 0;
        if(execute_test) {
            tests[i].callback();
            if(unit_test_name)
                return 0;
        }
    }

    return 0;
}
