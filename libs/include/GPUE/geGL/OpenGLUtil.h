#pragma once

#include<geGL/OpenGL.h>
#include<iostream>

namespace ge{
  namespace gl{
    GEGL_EXPORT std::string translateDebugSource  (GLenum source  );
    GEGL_EXPORT std::string translateDebugType    (GLenum type    );
    GEGL_EXPORT std::string translateDebugSeverity(GLenum severity);

    GEGL_EXPORT std::string translateBufferTarget (GLenum     target );
    GEGL_EXPORT std::string translateBufferBinding(GLenum     binding);
    GEGL_EXPORT GLenum      bufferTarget2Binding  (GLenum     target );
    GEGL_EXPORT GLenum      bufferBinding2Target  (GLenum     binding);
    GEGL_EXPORT bool        areBufferFlagsMutable (GLbitfield flags  );

    GEGL_EXPORT std::string translateFramebufferComponentType(GLenum type);
    GEGL_EXPORT std::string translateFramebufferType   (GLenum type);
    GEGL_EXPORT std::string translateFramebufferAttachment   (GLenum attachment);
    GEGL_EXPORT std::string translateFramebufferColorEncoding(GLenum type);
    GEGL_EXPORT std::string translateCubeMapFace(GLenum face);

    GEGL_EXPORT GLenum textureTarget2Binding(GLenum target );
    GEGL_EXPORT GLenum textureBinding2Target(GLenum binding);
    GEGL_EXPORT std::string translateTextureTarget     (GLenum target );
    GEGL_EXPORT std::string translateTextureBinding    (GLenum binding);
    GEGL_EXPORT std::string translateTextureCompareMode(GLint mode    );
    GEGL_EXPORT std::string translateTextureCompareFunc(GLint func    );
    GEGL_EXPORT std::string translateTextureFilter     (GLint filter  );
    GEGL_EXPORT std::string translateTextureWrap       (GLint wrap    );
    GEGL_EXPORT std::string translateTextureSwizzle    (GLint swizzle );
    GEGL_EXPORT std::string translateTextureChannelType(GLenum type   );
    GEGL_EXPORT unsigned internalFormatSize(GLenum internalFormat);
    GEGL_EXPORT std::string translateInternalFormat(GLenum internalFormat);
    GEGL_EXPORT std::string translateImageFormatCompatibilityType(GLenum type);

    GEGL_EXPORT std::string translateAttribType(GLenum    type   );
    GEGL_EXPORT std::string translateBoolean   (GLboolean boolean);
    GEGL_EXPORT unsigned    getTypeSize        (GLenum    type   );

    GEGL_EXPORT std::string translateUniformType      (GLenum      type    );
    GEGL_EXPORT std::string translateBufferProperty   (GLenum      property);


    GEGL_EXPORT bool isSamplerType(GLenum type);
  }
}

