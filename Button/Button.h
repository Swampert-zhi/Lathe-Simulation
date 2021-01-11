#pragma once
#include "../include/shader.h"
#include <glm/gtc/matrix_transform.hpp>

class Button{
private:
    Shader buttonShader;
    glm::mat4 model;

	unsigned int buttonVAO, buttonVBO;
    unsigned int textureID;
public:
    Button(std::string path,int screenWidth,int screenHeight,int width=100,int height=80,int posX=0,int posY=0)
    :buttonShader(Shader("../Button/Button.vs","../Button/Button.fs")){
        float normW=float(width)/screenWidth,normH=float(height)/screenHeight;
        float vertices[] = {
             normW, normH, 0.0f,   1.0f, 1.0f,
            -normW, normH, 0.0f,   0.0f, 1.0f,
             normW,-normH, 0.0f,   1.0f, 0.0f,
             normW,-normH, 0.0f,   1.0f, 0.0f,
            -normW, normH, 0.0f,   0.0f, 1.0f,
            -normW,-normH, 0.0f,   0.0f, 0.0f
        };
        model=glm::mat4(1.0f);
        model=glm::translate(model,glm::vec3(float(posX) *2 / screenWidth, float(posY) * 2 / screenHeight, 0));

		glGenVertexArrays(1, &buttonVAO);
		glGenBuffers(1, &buttonVBO);

        glBindVertexArray(buttonVAO);
		glBindBuffer(GL_ARRAY_BUFFER, buttonVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);

		glGenTextures(1, &this->textureID);
		glBindTexture(GL_TEXTURE_2D, this->textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int widthTex, heightTex, nrChannels;
        unsigned char *data = stbi_load(path.c_str(), &widthTex, &heightTex, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex, heightTex, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            // glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        buttonShader.use();
        buttonShader.setMat4("model",model);
        buttonShader.setInt("texture1",0);
    }
    ~Button(){}
    void drawButton(){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->textureID);

		buttonShader.use();
        glBindVertexArray(buttonVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
    }
};