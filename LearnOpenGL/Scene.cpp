#include "Scene.h"

void Scene::addObject(SceneObject& obj) {
	objects.push_back(obj);
}

void Scene::addLightSource(SceneObject& lt) {
	lightSources.push_back(lt);
}

void Scene::render(Camera &cam, float lastFrame, int width_px, int height_px) {
    vec3 lightPos(1.2f, 1.0f, 2.0f); // TODO attach to light source

    mat4 model(1.0);
    model = rotate(model, lastFrame * radians(50.0f), vec3(0.5, 1.0, 0.0));
    const float radius = 10.0f;
    mat4 view = cam.getViewMatrix();
    mat4 projection(1.0);
    projection = perspective(radians(45.0f), ((float)width_px / (float)height_px), 0.1f, 100.0f);

    vector<SceneObject> rdObj = vector<SceneObject>(objects);
    rdObj.insert(rdObj.end(), lightSources.begin(), lightSources.end());

    for (unsigned int i = 0; i < rdObj.size(); i++) {
        SceneObject& obj = rdObj[i];
        glUseProgram(obj.shaderProgram);
        int varLoc = glGetUniformLocation(obj.shaderProgram, "model");
        mat4 model_r(1.0);
        if (obj.type == light) {
            model_r = mat4(1.0);
            model_r = translate(model_r, lightPos);
            model_r = scale(model_r, vec3(0.2f));
        }
        else
            model_r = model;
        glUniformMatrix4fv(varLoc, 1, GL_FALSE, glm::value_ptr(model_r));
        varLoc = glGetUniformLocation(obj.shaderProgram, "view");
        glUniformMatrix4fv(varLoc, 1, GL_FALSE, glm::value_ptr(view));
        varLoc = glGetUniformLocation(obj.shaderProgram, "projection");
        glUniformMatrix4fv(varLoc, 1, GL_FALSE, glm::value_ptr(projection));

        draw(obj, lastFrame);
    }
}