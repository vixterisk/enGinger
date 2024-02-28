template<class F>
void checkCondition(bool condition, F errorHandler, const char* errorMessage)
{
    if (!condition) {
        errorHandler();
        throw std::exception(errorMessage);
    }
}