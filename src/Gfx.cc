#include "Gfx.hh"
#include <spdlog/spdlog.h>
#include <filesystem>
#include <vector>
#include <cmath>

GLenum glCheckError_(const char *file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }
        spdlog::error("{} | {} ({})", error, file, line);
    }
    return errorCode;
}

unsigned int Gfx::shaderFromString(char const *data, GLenum type) {
    unsigned int shader = gl(CreateShader(type));
    gl(ShaderSource(shader, 1, &data, NULL));
    gl(CompileShader(shader));
    int success;
    char infoLog[512];
    gl(GetShaderiv(shader, GL_COMPILE_STATUS, &success));
    if (!success) {
        gl(GetShaderInfoLog(shader, 512, NULL, infoLog));
        spdlog::error("error compiling shader: {}", infoLog);
        return 0;
    }
    return shader;
}

struct NamedImage {
    sf::Image image;
    std::string name;
};

struct ImageNode {
    NamedImage const *image = NULL;
    ImageNode *right = NULL;
    ImageNode *down = NULL;
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
};

ImageNode const *addImage(ImageNode *root, NamedImage const *image) {
    sf::Vector2u size = image->image.getSize();
    if (root->image == NULL) {
        if ((int)size.x <= root->width && (int)size.y <= root->height) {
            root->image = image;
            root->right = new ImageNode;
            root->right->x = root->x + size.x + 1;
            root->right->y = root->y;
            root->right->width = root->width - size.x - 2;
            root->right->height = size.y;
            root->down = new ImageNode;
            root->down->x = root->x;
            root->down->y = root->y + size.y + 1;
            root->down->width = root->width;
            root->down->height = root->height - size.y - 2;
            return root;
        }
        return NULL;
    }
    ImageNode const *result = addImage(root->right, image);
    if (result) return result;
    return addImage(root->down, image);
}

void deleteTree(ImageNode *root) {
    if (root->right) deleteTree(root->right);
    if (root->down) deleteTree(root->down);
    delete root->right;
    delete root->down;
}

bool compareImages(NamedImage const &a, NamedImage const &b) {
    sf::Vector2u aSize = a.image.getSize();
    sf::Vector2u bSize = b.image.getSize();
    return fmax(aSize.x, aSize.y) > fmax(bSize.x, bSize.y);
}

bool Gfx::createAtlas(
    char const *folder,
    char const *outImage,
    char const *outXml,
    unsigned int maxWidth,
    unsigned int maxHeight
) {
    spdlog::info(
        "creating atlas from images in {} for image {} and xml {} with max "
        "dimensions ({}, {})",
        folder,
        outImage,
        outXml,
        maxWidth,
        maxHeight
    );
    std::vector<NamedImage> images;
    for (const auto& dir: std::filesystem::recursive_directory_iterator(folder)
    ) {
        char const *file = dir.path().c_str();
        NamedImage image;
        image.name = file;
        if (image.image.loadFromFile(file)) {
            spdlog::info("adding {} to atlas", file);
            images.push_back(std::move(image));
        }
    }
    if (images.empty()) {
        spdlog::warn("no images in sprites folder for atlas");
        return false;
    }
    std::sort(images.begin(), images.end(), compareImages);
    sf::Image canvas;
    canvas.create(maxWidth, maxHeight, sf::Color(0, 0, 0, 0));
    ImageNode root;
    root.width = maxWidth;
    root.height = maxHeight;
    bool success = true;
    for (NamedImage const &image: images) {
        ImageNode const *placement = addImage(&root, &image);
        if (placement) {
            canvas.copy(image.image, placement->x, placement->y);
        } else {
            spdlog::warn("couldn't fit {} in atlas", image.name);
            success = false;
            break;
        }
    }
    deleteTree(&root);
    canvas.saveToFile(outImage);
    return success;
}
