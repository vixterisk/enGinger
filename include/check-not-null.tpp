template<class T, class F>
T checkNotNull(T value, F errorHandler, std::string errorMessage)
{
    if (&value == nullptr) {
        errorHandler();
        throw std::exception((const char*&)errorMessage);
    }
    return value;
}

template<class T, class F>
T* checkNotNull(T* value, F errorHandler, std::string errorMessage)
{
    if (value == nullptr) {
        errorHandler();
        throw std::exception((const char*&)errorMessage);
    }
    return value;
}