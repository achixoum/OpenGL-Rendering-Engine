#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include "../../glm/glm.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Shader {
private:
    //the unique ids of the shaders
    unsigned int ShaderID, vertex, fragment;

    void checkCompilerErrors(unsigned int shader, const string& type) {
        int success;
        char infoLog[512];
        if (type == "PROGRAM") {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 512, NULL, infoLog);
                cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
            }
        }
        else {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 512, NULL, infoLog);
                cout<<infoLog<<endl;
            }
        }
    }

    void compile(const char* vertexShaderCode, const char* fragmentShaderCode) {
        int success;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexShaderCode, NULL);
        glCompileShader(vertex);

        checkCompilerErrors(vertex, "VERTEX");

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        checkCompilerErrors(fragment, "FRAGMENT");
    }
public:
    Shader(const char* vertexShaderFilename, const char* fragmentShaderFilename) {
        string vertexCode;
        string fragmentCode;
        ifstream vertexShaderFile;
        ifstream fragmentShaderFile;
        try {
            // open files
            vertexShaderFile.open(vertexShaderFilename);
            fragmentShaderFile.open(fragmentShaderFilename);
            stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vertexShaderFile.rdbuf();
            fShaderStream << fragmentShaderFile.rdbuf();
            // close file handlers
            vertexShaderFile.close();
            fragmentShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (ifstream::failure& e) {
            cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << endl;
        }

        compile(vertexCode.c_str(), fragmentCode.c_str());

        ShaderID = glCreateProgram();
        glAttachShader(ShaderID, vertex);
        glAttachShader(ShaderID, fragment);
        glLinkProgram(ShaderID);

        checkCompilerErrors(ShaderID, "PROGRAM");
    }

	void use() {
        glUseProgram(ShaderID);
    }

    void deleteProgram() const {
        glDeleteProgram(ShaderID);
    }

    void deleteVertexShader() const {
        glDeleteShader(vertex);
    }

    void deleteFragmentShader() const {
        glDeleteShader(fragment);
    }

    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ShaderID, name.c_str()), (int)value);
    }

    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ShaderID, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ShaderID, name.c_str()), value);
    }

    void setVec2(const std::string &name, const glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(ShaderID, name.c_str()), 1, &value[0]);
    }

    void setVec2(const std::string &name, float x, float y) const {
        glUniform2f(glGetUniformLocation(ShaderID, name.c_str()), x, y);
    }

    void setVec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(ShaderID, name.c_str()), 1, &value[0]);
    }

    void setVec3(const std::string &name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(ShaderID, name.c_str()), x, y, z);
    }

    void setVec4(const std::string &name, const glm::vec4 &value) const {
        glUniform4fv(glGetUniformLocation(ShaderID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(ShaderID, name.c_str()), x, y, z, w);
    }

    void setMat2(const std::string &name, const glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat3(const std::string &name, const glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};



#endif //SHADER_H
