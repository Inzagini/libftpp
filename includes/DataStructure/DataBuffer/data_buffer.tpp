#ifndef LIBFTPP_INCLUDES_DATASTRUCTURE_DATABUFFER_DATA_BUFFER_TPP
# define LIBFTPP_INCLUDES_DATASTRUCTURE_DATABUFFER_DATA_BUFFER_TPP

template <typename T> DataBuffer& DataBuffer::operator<<(const T& dataValue) {
  static_assert(std::is_trivially_copyable_v<T>,
                "Type must by trivially copyable");
  writeBytes(reinterpret_cast<const char*>(&dataValue), sizeof(T));
  return *this;
}

template <typename T> DataBuffer& DataBuffer::operator>>(T& dataValue) {
  static_assert(std::is_trivially_copyable_v<T>,
                "Type must by trivially copyable");
  readBytes(reinterpret_cast<char*>(&dataValue), sizeof(T));
  return *this;
}
#endif // LIBFTPP_INCLUDES_DATASTRUCTURE_DATABUFFER_DATA_BUFFER_TPP
