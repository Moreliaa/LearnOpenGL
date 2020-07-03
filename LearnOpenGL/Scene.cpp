#include "Scene.h"

void Scene::addObject(SceneObject& obj) {
	objects.push_back(obj);
}

void Scene::addLightSource(SceneObject& lt) {
	lightSources.push_back(lt);
}

void Scene::render(Camera &cam, float lastFrame, int width_px, int height_px) {
    mat4 view = cam.getViewMatrix();
    mat4 projection(1.0);
    projection = perspective(radians(45.0f), ((float)width_px / (float)height_px), 0.1f, 100.0f);

    vector<SceneObject> rdObj = vector<SceneObject>(objects);
    rdObj.insert(rdObj.end(), lightSources.begin(), lightSources.end());

    for (unsigned int i = 0; i < rdObj.size(); i++) {
        SceneObject& obj = rdObj[i];
        mat4 model = getModelMatrix(obj, lastFrame);
        glUseProgram(obj.shaderProgram);
        int varLoc = glGetUniformLocation(obj.shaderProgram, "model");
        glUniformMatrix4fv(varLoc, 1, GL_FALSE, glm::value_ptr(model));
        varLoc = glGetUniformLocation(obj.shaderProgram, "view");
        glUniformMatrix4fv(varLoc, 1, GL_FALSE, glm::value_ptr(view));
        varLoc = glGetUniformLocation(obj.shaderProgram, "projection");
        glUniformMatrix4fv(varLoc, 1, GL_FALSE, glm::value_ptr(projection));

        draw(obj, lastFrame);
    }
}