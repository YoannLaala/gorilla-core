#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "dictionary_data.hpp"

/******************************************************************************
**  Forward Declaration
******************************************************************************/
namespace Gorilla
{
    class Dictionary;
    class StreamReader;
    class StreamWriter;
}

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class DictionaryNode
    {
        friend class Dictionary;
        friend class DictionaryBinaryStream;

    public:
        static const DictionaryNode INVALID;

    private:
        DictionaryNode(Dictionary *dictionary, DictionaryData *data);

    public:
        DictionaryNode();
        ~DictionaryNode();

        inline bool                         is_valid() const;
        inline bool                         is_empty() const;

        inline DictionaryData::Format       get_format() const;
        inline DictionaryData::Format       get_buffer_format() const;
        const char*                         get_name() const;

        bool                                next();
        DictionaryNode                      get_next() const;
        DictionaryNode                      get_first_child() const;
        DictionaryNode                      get_last_child() const;

        DictionaryNode                      find(const char *name) const;
        DictionaryNode                      add(const char *name = nullptr);
        void                                remove(const char *name);

        // type check
        inline bool                         is_bool() const;
        inline bool                         is_uint8() const;
        inline bool                         is_uint16() const;
        inline bool                         is_uint32() const;
        inline bool                         is_uint64() const;
        inline bool                         is_int8() const;
        inline bool                         is_int16() const;
        inline bool                         is_int32() const;
        inline bool                         is_int64() const;
        inline bool                         is_float32() const;
        inline bool                         is_float64() const;
        inline bool                         is_string() const;
        inline bool                         is_buffer() const;
        inline bool                         is_object() const;
        inline bool                         is_array() const;
        inline bool                         is_null() const;

        // get_value
        inline bool                         get_bool() const;
        inline uint8_t                      get_uint8() const;
        inline uint16_t                     get_uint16() const;
        inline uint32_t                     get_uint32() const;
        inline uint64_t                     get_uint64() const;
        inline int8_t                       get_int8() const;
        inline int16_t                      get_int16() const;
        inline int32_t                      get_int32() const;
        inline int64_t                      get_int64() const;
        inline float                        get_float32() const;
        inline double                       get_float64() const;
        inline const char*                  get_string() const;

        // set_value
        void                                set_bool(bool value);
        void                                set_uint8(uint8_t value);
        void                                set_uint16(uint16_t value);
        void                                set_uint32(uint32_t value);
        void                                set_uint64(uint64_t value);
        void                                set_int8(int8_t value);
        void                                set_int16(int16_t value);
        void                                set_int32(int32_t value);
        void                                set_int64(int64_t value);
        void                                set_float32(float value);
        void                                set_float64(double value);
        void                                set_string(const char *value, uint32_t length);
        void                                set_object();
        void                                set_array();
        void                                set_null();

        // get_buffer
        void                                get_buffer_bool(const bool **buffer, uint32_t *count) const;
        void                                get_buffer_uint8(const uint8_t **buffer, uint32_t *count) const;
        void                                get_buffer_uint16(const uint16_t **buffer, uint32_t *count) const;
        void                                get_buffer_uint32(const uint32_t **buffer, uint32_t *count) const;
        void                                get_buffer_uint64(const uint64_t **buffer, uint32_t *count) const;
        void                                get_buffer_int8(const int8_t **buffer, uint32_t *count) const;
        void                                get_buffer_int16(const int16_t **buffer, uint32_t *count) const;
        void                                get_buffer_int32(const int32_t **buffer, uint32_t *count) const;
        void                                get_buffer_int64(const int64_t **buffer, uint32_t *count) const;
        void                                get_buffer_float32(const float **buffer, uint32_t *count) const;
        void                                get_buffer_float64(const double **buffer, uint32_t *count) const;
        void                                get_buffer_string(Vector<const char*> *buffer) const;

        // set_buffer
        inline bool*                        set_buffer_bool(uint32_t count);
        inline uint8_t*                     set_buffer_uint8(uint32_t count);
        inline uint16_t*                    set_buffer_uint16(uint32_t count);
        inline uint32_t*                    set_buffer_uint32(uint32_t count);
        inline uint64_t*                    set_buffer_uint64(uint32_t count);
        inline int8_t*                      set_buffer_int8(uint32_t count);
        inline int16_t*                     set_buffer_int16(uint32_t count);
        inline int32_t*                     set_buffer_int32(uint32_t count);
        inline int64_t*                     set_buffer_int64(uint32_t count);
        inline float*                       set_buffer_float32(uint32_t count);
        inline double*                      set_buffer_float64(uint32_t count);

        inline void                         set_buffer_bool(const bool *buffer, uint32_t count);
        inline void                         set_buffer_uint8(const uint8_t *buffer, uint32_t count);
        inline void                         set_buffer_uint16(const uint16_t *buffer, uint32_t count);
        inline void                         set_buffer_uint32(const uint32_t *buffer, uint32_t count);
        inline void                         set_buffer_uint64(const uint64_t *buffer, uint32_t count);
        inline void                         set_buffer_int8(const int8_t *buffer, uint32_t count);
        inline void                         set_buffer_int16(const int16_t *buffer, uint32_t count);
        inline void                         set_buffer_int32(const int32_t *buffer, uint32_t count);
        inline void                         set_buffer_int64(const int64_t *buffer, uint32_t count);
        inline void                         set_buffer_float32(const float *buffer, uint32_t count);
        inline void                         set_buffer_float64(const double *buffer, uint32_t count);
        void                                set_buffer_string(const char **buffer, uint32_t count);
        void                                set_buffer_string(const String *buffer, uint32_t count);

        // operator
        inline bool                         operator==(const DictionaryNode& other) const;
        inline bool                         operator!=(const DictionaryNode& other) const;
        DictionaryNode                      operator[](const char *name);
        inline void                         operator=(bool value);
        inline void                         operator=(uint8_t value);
        inline void                         operator=(uint16_t value);
        inline void                         operator=(uint32_t value);
        inline void                         operator=(uint64_t value);
        inline void                         operator=(int8_t value);
        inline void                         operator=(int16_t value);
        inline void                         operator=(int32_t value);
        inline void                         operator=(int64_t value);
        inline void                         operator=(float value);
        inline void                         operator=(double value);
        inline void                         operator=(const char *value);
        inline DictionaryNode&              operator++();
        inline DictionaryNode               operator++(int /*value*/);
        inline                              operator bool() const;
        inline                              operator uint8_t() const;
        inline                              operator uint16_t() const;
        inline                              operator uint32_t() const;
        inline                              operator uint64_t() const;
        inline                              operator int8_t() const;
        inline                              operator int16_t() const;
        inline                              operator int32_t() const;
        inline                              operator int64_t() const;
        inline                              operator float() const;
        inline                              operator double() const;
        inline                              operator const char*() const;

    private:
        void  set_buffer_string_internal(const char *buffer, uint32_t length, uint32_t *offset);
        void* set_buffer_internal(DictionaryData::Format format, const void *buffer, uint32_t element_count, uint32_t element_size);

    private:
        Dictionary* m_dictionary;
        DictionaryData* m_data;
    };

    //! @brief      is_valid
    bool DictionaryNode::is_valid() const
    {
        return m_data != nullptr;
    }

    //! @brief      is_empty
    bool DictionaryNode::is_empty() const
    {
        return get_first_child() == DictionaryNode::INVALID;
    }

    //! @brief      get_format
    DictionaryData::Format DictionaryNode::get_format() const
    {
        return m_data->get_format();
    }

    //! @brief      get_buffer_format
    DictionaryData::Format DictionaryNode::get_buffer_format() const
    {
        return reinterpret_cast<TDictionaryData<DictionaryData::Format::BUFFER>*>(m_data)->Format;
    }

    //! @brief      is_bool
    bool DictionaryNode::is_bool() const
    {
        return m_data->get_format() == DictionaryData::Format::BOOLEAN;
    }

    //! @brief      is_uint8_t
    bool DictionaryNode::is_uint8() const
    {
        return m_data->get_format() == DictionaryData::Format::UINTEGER8;
    }

    //! @brief      is_uint16_t
    bool DictionaryNode::is_uint16() const
    {
        return m_data->get_format() == DictionaryData::Format::UINTEGER16;
    }

    //! @brief      is_uint32_t
    bool DictionaryNode::is_uint32() const
    {
        return m_data->get_format() == DictionaryData::Format::UINTEGER32;
    }

    //! @brief      is_uint64_t
    bool DictionaryNode::is_uint64() const
    {
        return m_data->get_format() == DictionaryData::Format::UINTEGER64;
    }

    //! @brief      is_int8
    bool DictionaryNode::is_int8() const
    {
        return m_data->get_format() == DictionaryData::Format::INTEGER8;
    }

    //! @brief      is_int16
    bool DictionaryNode::is_int16() const
    {
        return m_data->get_format() == DictionaryData::Format::INTEGER16;
    }

    //! @brief      is_int32
    bool DictionaryNode::is_int32() const
    {
        return m_data->get_format() == DictionaryData::Format::INTEGER32;
    }

    //! @brief      is_int64
    bool DictionaryNode::is_int64() const
    {
        return m_data->get_format() == DictionaryData::Format::INTEGER64;
    }

    //! @brief      is_float32
    bool DictionaryNode::is_float32() const
    {
        return m_data->get_format() == DictionaryData::Format::FLOAT32;
    }

    //! @brief      is_float64
    bool DictionaryNode::is_float64() const
    {
        return m_data->get_format() == DictionaryData::Format::FLOAT64;
    }

    //! @brief      is_string
    bool DictionaryNode::is_string() const
    {
        return m_data->get_format() == DictionaryData::Format::STRING;
    }

    //! @brief      is_buffer
    bool DictionaryNode::is_buffer() const
    {
        return m_data->get_format() == DictionaryData::Format::BUFFER;
    }

    //! @brief      is_object
    bool DictionaryNode::is_object() const
    {
        return m_data->get_format() == DictionaryData::Format::OBJECT;
    }

    //! @brief      is_array
    bool DictionaryNode::is_array() const
    {
        return m_data->get_format() == DictionaryData::Format::ARRAY;
    }

    //! @brief      is_null
    bool DictionaryNode::is_null() const
    {
        return m_data->get_format() == DictionaryData::Format::ZERO;
    }

    //! @brief      get_bool
    bool DictionaryNode::get_bool() const
    {
        return reinterpret_cast<TDictionaryData<DictionaryData::Format::BOOLEAN>*>(m_data)->Value;
    }

    //! @brief      get_uint8
    uint8_t DictionaryNode::get_uint8() const
    {
        return reinterpret_cast<TDictionaryData<DictionaryData::Format::UINTEGER8>*>(m_data)->Value;
    }

    //! @brief      get_uint16
    uint16_t DictionaryNode::get_uint16() const
    {
        return reinterpret_cast<TDictionaryData<DictionaryData::Format::UINTEGER16>*>(m_data)->Value;
    }

    //! @brief      get_uint32
    uint32_t DictionaryNode::get_uint32() const
    {
        return reinterpret_cast<TDictionaryData<DictionaryData::Format::UINTEGER32>*>(m_data)->Value;
    }

    //! @brief      get_uint64
    uint64_t DictionaryNode::get_uint64() const
    {
        return reinterpret_cast<TDictionaryData<DictionaryData::Format::UINTEGER64>*>(m_data)->Value;
    }

    //! @brief      get_int8
    int8_t DictionaryNode::get_int8() const
    {
        return reinterpret_cast<TDictionaryData<DictionaryData::Format::INTEGER8>*>(m_data)->Value;
    }

    //! @brief      get_int16
    int16_t DictionaryNode::get_int16() const
    {
        return reinterpret_cast<TDictionaryData<DictionaryData::Format::INTEGER16>*>(m_data)->Value;
    }

    //! @brief      get_int32
    int32_t DictionaryNode::get_int32() const
    {
        return reinterpret_cast<TDictionaryData<DictionaryData::Format::INTEGER32>*>(m_data)->Value;
    }

    //! @brief      get_int64
    int64_t DictionaryNode::get_int64() const
    {
        return reinterpret_cast<TDictionaryData<DictionaryData::Format::INTEGER64>*>(m_data)->Value;
    }

    //! @brief      get_float32
    float DictionaryNode::get_float32() const
    {
        return reinterpret_cast<TDictionaryData<DictionaryData::Format::FLOAT32>*>(m_data)->Value;
    }

    //! @brief      get_float64
    double DictionaryNode::get_float64() const
    {
        return reinterpret_cast<TDictionaryData<DictionaryData::Format::FLOAT64>*>(m_data)->Value;
    }

    //! @brief      get_string
    const char* DictionaryNode::get_string() const
    {
        return reinterpret_cast<const char*>((uint8_t*)m_data + sizeof(TDictionaryData<DictionaryData::Format::STRING>));
    }

    //! @brief      set_buffer_bool
    bool* DictionaryNode::set_buffer_bool(uint32_t count)
    {
        return (bool*)set_buffer_internal(DictionaryData::Format::BOOLEAN, nullptr, count, count);
    }

    //! @brief      set_buffer_uint8
    uint8_t* DictionaryNode::set_buffer_uint8(uint32_t count)
    {
        return (uint8_t*)set_buffer_internal(DictionaryData::Format::UINTEGER8, nullptr, count, count);
    }

    //! @brief      set_buffer_uint16
    uint16_t* DictionaryNode::set_buffer_uint16(uint32_t count)
    {
        return (uint16_t*)set_buffer_internal(DictionaryData::Format::UINTEGER16, nullptr, count, sizeof(uint16_t));
    }

    //! @brief      set_buffer_uint32
    uint32_t* DictionaryNode::set_buffer_uint32(uint32_t count)
    {
        return (uint32_t*)set_buffer_internal(DictionaryData::Format::UINTEGER32, nullptr, count, sizeof(uint32_t));
    }

    //! @brief      set_buffer_uint64
    uint64_t* DictionaryNode::set_buffer_uint64(uint32_t count)
    {
        return (uint64_t*)set_buffer_internal(DictionaryData::Format::UINTEGER64, nullptr, count, sizeof(uint64_t));
    }

    //! @brief      set_buffer_int8
    int8_t* DictionaryNode::set_buffer_int8(uint32_t count)
    {
        return (int8_t*)set_buffer_internal(DictionaryData::Format::INTEGER8, nullptr, count, count);
    }

    //! @brief      set_buffer_int16
    int16_t* DictionaryNode::set_buffer_int16(uint32_t count)
    {
        return (int16_t*)set_buffer_internal(DictionaryData::Format::INTEGER16, nullptr, count, sizeof(int16_t));
    }

    //! @brief      set_buffer_int32
    int32_t* DictionaryNode::set_buffer_int32(uint32_t count)
    {
        return (int32_t*)set_buffer_internal(DictionaryData::Format::INTEGER32, nullptr, count, sizeof(int32_t));
    }

    //! @brief      set_buffer_int64
    int64_t* DictionaryNode::set_buffer_int64(uint32_t count)
    {
        return (int64_t*)set_buffer_internal(DictionaryData::Format::INTEGER64, nullptr, count, sizeof(int64_t));
    }

    //! @brief      set_buffer_float32
    float* DictionaryNode::set_buffer_float32(uint32_t count)
    {
        return (float*)set_buffer_internal(DictionaryData::Format::FLOAT32, nullptr, count, sizeof(float));
    }

    //! @brief      set_buffer_float64
    double* DictionaryNode::set_buffer_float64(uint32_t count)
    {
        return (double*)set_buffer_internal(DictionaryData::Format::FLOAT64, nullptr, count, sizeof(double));
    }

    //! @brief      set_buffer_bool
    void DictionaryNode::set_buffer_bool(const bool *buffer, uint32_t count)
    {
        set_buffer_internal(DictionaryData::Format::BOOLEAN, buffer, count, count);
    }

    //! @brief      set_buffer_uint8
    void DictionaryNode::set_buffer_uint8(const uint8_t *buffer, uint32_t count)
    {
        set_buffer_internal(DictionaryData::Format::UINTEGER8, buffer, count, count);
    }

    //! @brief      set_buffer_uint16
    void DictionaryNode::set_buffer_uint16(const uint16_t *buffer, uint32_t count)
    {
        set_buffer_internal(DictionaryData::Format::UINTEGER16, buffer, count, sizeof(uint16_t));
    }

    //! @brief      set_buffer_uint32
    void DictionaryNode::set_buffer_uint32(const uint32_t *buffer, uint32_t count)
    {
        set_buffer_internal(DictionaryData::Format::UINTEGER32, buffer, count, sizeof(uint32_t));
    }

    //! @brief      set_buffer_uint64
    void DictionaryNode::set_buffer_uint64(const uint64_t *buffer, uint32_t count)
    {
        set_buffer_internal(DictionaryData::Format::UINTEGER64, buffer, count, sizeof(uint64_t));
    }

    //! @brief      set_buffer_int8
    void DictionaryNode::set_buffer_int8(const int8_t *buffer, uint32_t count)
    {
        set_buffer_internal(DictionaryData::Format::INTEGER8, buffer, count, count);
    }

    //! @brief      set_buffer_int16
    void DictionaryNode::set_buffer_int16(const int16_t *buffer, uint32_t count)
    {
        set_buffer_internal(DictionaryData::Format::INTEGER16, buffer, count, sizeof(int16_t));
    }

    //! @brief      set_buffer_int32
    void DictionaryNode::set_buffer_int32(const int32_t *buffer, uint32_t count)
    {
        set_buffer_internal(DictionaryData::Format::INTEGER32, buffer, count, sizeof(int32_t));
    }

    //! @brief      set_buffer_int64
    void DictionaryNode::set_buffer_int64(const int64_t *buffer, uint32_t count)
    {
        set_buffer_internal(DictionaryData::Format::INTEGER64, buffer, count, sizeof(int64_t));
    }

    //! @brief      set_buffer_float32
    void DictionaryNode::set_buffer_float32(const float *buffer, uint32_t count)
    {
        set_buffer_internal(DictionaryData::Format::FLOAT32, buffer, count, sizeof(float));
    }

    //! @brief      set_buffer_float64
    void DictionaryNode::set_buffer_float64(const double *buffer, uint32_t count)
    {
        set_buffer_internal(DictionaryData::Format::FLOAT64, buffer, count, sizeof(double));
    }

    //! @brief      operator==
    bool DictionaryNode::operator==(const DictionaryNode& other) const
    {
        return m_data == other.m_data;
    }

    //! @brief      operator!=
    bool DictionaryNode::operator!=(const DictionaryNode& other) const
    {
        return m_data != other.m_data;
    }

    //! @brief      operator=
    void DictionaryNode::operator=(bool value)
    {
        return set_bool(value);
    }

    //! @brief      operator=
    void DictionaryNode::operator=(uint8_t value)
    {
        return set_uint8(value);
    }

    //! @brief      operator=
    void DictionaryNode::operator=(uint16_t value)
    {
        return set_uint16(value);
    }

    //! @brief      operator=
    void DictionaryNode::operator=(uint32_t value)
    {
        return set_uint32(value);
    }

    //! @brief      operator=
    void DictionaryNode::operator=(uint64_t value)
    {
        return set_uint64(value);
    }

    //! @brief      operator=
    void DictionaryNode::operator=(int8_t value)
    {
        return set_int8(value);
    }

    //! @brief      operator=
    void DictionaryNode::operator=(int16_t value)
    {
        return set_int16(value);
    }

    //! @brief      operator=
    void DictionaryNode::operator=(int32_t value)
    {
        return set_int32(value);
    }

    //! @brief      operator=
    void DictionaryNode::operator=(int64_t value)
    {
        return set_int64(value);
    }

    //! @brief      operator=
    void DictionaryNode::operator=(float value)
    {
        return set_float32(value);
    }

    //! @brief      operator=
    void DictionaryNode::operator=(double value)
    {
        return set_float64(value);
    }

    //! @brief      operator=
    void DictionaryNode::operator=(const char *value)
    {
        return set_string(value, (uint32_t)STRLEN(value));
    }

    //! @brief      operator++
    DictionaryNode& DictionaryNode::operator++()
    {
        next();
        return *this;
    }

    //! @brief      operator++
    DictionaryNode DictionaryNode::operator++(int /*value*/)
    {
        return get_next();
    }

    //! @brief      operator bool
    DictionaryNode::operator bool() const
    {
        return get_bool();
    }

    //! @brief      operator uint8_t
    DictionaryNode::operator uint8_t() const
    {
        return get_uint8();
    }

    //! @brief      operator uint16_t
    DictionaryNode::operator uint16_t() const
    {
        return get_uint16();
    }

    //! @brief      operator uint32_t
    DictionaryNode::operator uint32_t() const
    {
        return get_uint32();
    }

    //! @brief      operator uint64_t
    DictionaryNode::operator uint64_t() const
    {
        return get_uint64();
    }

    //! @brief      operator int8_t
    DictionaryNode::operator int8_t() const
    {
        return get_int8();
    }

    //! @brief      operator int16_t
    DictionaryNode::operator int16_t() const
    {
        return get_int16();
    }

    //! @brief      operator int32_t
    DictionaryNode::operator int32_t() const
    {
        return get_int32();
    }

    //! @brief      operator int64_t
    DictionaryNode::operator int64_t() const
    {
        return get_int64();
    }

    //! @brief      operator float
    DictionaryNode::operator float() const
    {
        return get_float32();
    }

    //! @brief      operator double
    DictionaryNode::operator double() const
    {
        return get_float64();
    }

    //! @brief      operator const char*
    DictionaryNode::operator const char*() const
    {
        return get_string();
    }
}
