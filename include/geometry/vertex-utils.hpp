#include <glad/glad.h>

struct Position
{
    GLfloat x;
    GLfloat y;
    GLfloat z;

    Position(GLfloat _x = 0.0f, GLfloat _y = 0.0f, GLfloat _z = 0.0f)
    {
        x = _x;
        y = _y;
        z = _z;
    }
};

struct Color
{
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;

    Color() {}
    static constexpr Color black() { return {0, 0, 0}; }
    static constexpr Color white() { return {1.0f, 1.0f, 1.0f}; }
    static constexpr Color red() { return {1.0f, 0, 0}; }
    static constexpr Color green() { return {0, 1.0f, 0}; }
    static constexpr Color blue() { return {0, 0, 1.0f}; }
    static constexpr Color yellow() { return {1.0f, 1.0f, 0}; }
    static constexpr Color magenta() { return {1.0f, 0, 1.0f}; }
    static constexpr Color cyan() { return {0, 1.0f, 1.0f}; }
    constexpr Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) : r(r), g(g), b(b), a(a) {}
};

struct UV
{
    GLfloat u;
    GLfloat v;
    UV(GLfloat _u = 0.0f, GLfloat _v = 0.0f)
    {
        u = _u;
        v = _v;
    }
};

struct Vertex
{
    Position position;
    Color color;
    UV uv;
    Position normals;

    Vertex(Position _position, Color _color, UV _uv, Position _normals)
    {
        position = _position;
        color = _color;
        uv = _uv;
        normals = _normals;
    }
    //static Layout[] getLayout() {
    //    retun Layout[] = {
    //        {3, GL_FLOAT, false}
    //    }
    //}
};