#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include "logutils.h"

#define VERBOSE_LOGGING false

extern "C" {

jstring Java_com_stetcho_basicimageprocessing_MainActivity_naGetTitle(JNIEnv *env, jobject) {
    return env->NewStringUTF("Basic Image Processing");
}

static int rgb_clamp(int value) {
    if (value > 255) {
        return 255;
    }

    if (value < 0) {
        return 0;
    }

    return value;
}

void Java_com_stetcho_basicimageprocessing_MainActivity_naToGrayscale(JNIEnv *env,
                                                                     jclass clazz,
                                                                     jobject pSrcBitmap) {
    int srcResult;
    AndroidBitmapInfo srcInfo;
    void *srcPixels;

    if ((srcResult = AndroidBitmap_getInfo(env, pSrcBitmap, &srcInfo)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed! error = %d", srcResult);
        return;
    }

    if (srcInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Invalid bitmap format. It should be RGBA");
        return;
    }

    if ((srcResult = AndroidBitmap_lockPixels(env, pSrcBitmap, &srcPixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed! error = %d", srcResult);
    }

    int w, h, red, green, blue, gray;
    uint32_t *line;

    for (h = 0; h < srcInfo.height; h++) {
        line = (uint32_t *) srcPixels;

        for (w = 0; w < srcInfo.width; w++) {
            // extract the RGB values from the pixel
            red = (int) ((line[w] & 0x00FF0000) >> 16);
            green = (int) ((line[w] & 0x0000FF00) >> 8);
            blue = (int) (line[w] & 0x00000FF);

            gray = 0.299 * red + 0.587 * green + 0.114 * blue;

            red = gray;
            green = gray;
            blue = gray;

            // set the new pixel back in
            line[w] =
                    ((red << 16) & 0x00FF0000) |
                    ((green << 8) & 0x0000FF00) |
                    (blue & 0x000000FF);

        }
        srcPixels = (char *) srcPixels + srcInfo.stride;
    }
    AndroidBitmap_unlockPixels(env, pSrcBitmap);
}

void Java_com_stetcho_basicimageprocessing_MainActivity_naToBlackWhite(JNIEnv *env,
                                                                      jclass clazz,
                                                                      jobject pSrcBitmap) {
    int srcResult;
    AndroidBitmapInfo srcInfo;
    void *srcPixels;

    if ((srcResult = AndroidBitmap_getInfo(env, pSrcBitmap, &srcInfo)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed! error = %d", srcResult);
        return;
    }

    if (srcInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Invalid bitmap format. It should be RGBA");
        return;
    }

    if ((srcResult = AndroidBitmap_lockPixels(env, pSrcBitmap, &srcPixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed! error = %d", srcResult);
    }

    int w, h, red, green, blue;
    uint32_t *line;

    int threshold = 150;
    int gray;

    for (h = 0; h < srcInfo.height; h++) {
        line = (uint32_t *) srcPixels;

        for (w = 0; w < srcInfo.width; w++) {
            // extract the RGB values from the pixel
            red = (int) ((line[w] & 0x00FF0000) >> 16);
            green = (int) ((line[w] & 0x0000FF00) >> 8);
            blue = (int) (line[w] & 0x00000FF);

            gray = 0.299 * red + 0.587 * green + 0.114 * blue;

            if (gray > threshold) {
                gray = 255;
            } else {
                gray = 1;
            }

            red = gray;
            green = gray;
            blue = gray;

            // set the new pixel back in
            line[w] =
                    ((red << 16) & 0x00FF0000) |
                    ((green << 8) & 0x0000FF00) |
                    (blue & 0x000000FF);

        }
        srcPixels = (char *) srcPixels + srcInfo.stride;
    }
    AndroidBitmap_unlockPixels(env, pSrcBitmap);
}

void Java_com_stetcho_basicimageprocessing_MainActivity_naSetBrightness(JNIEnv *env,
                                                                       jclass clazz,
                                                                       jobject pSrcBitmap,
                                                                       jfloat brightnessValue) {
    int srcResult;
    AndroidBitmapInfo srcInfo;
    void *srcPixels;

    if ((srcResult = AndroidBitmap_getInfo(env, pSrcBitmap, &srcInfo)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed! error = %d", srcResult);
        return;
    }

    if (srcInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Invalid bitmap format. It should be RGBA");
        return;
    }

    if ((srcResult = AndroidBitmap_lockPixels(env, pSrcBitmap, &srcPixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed! error = %d", srcResult);
    }

    int w, h, red, green, blue;
    uint32_t *line;

    for (h = 0; h < srcInfo.height; h++) {
        line = (uint32_t *) srcPixels;

        for (w = 0; w < srcInfo.width; w++) {
            // extract the RGB values from the pixel
            red = (int) ((line[w] & 0x00FF0000) >> 16);
            green = (int) ((line[w] & 0x0000FF00) >> 8);
            blue = (int) (line[w] & 0x00000FF);

            // manipulate each value
            red = rgb_clamp((int) (red * brightnessValue));
            green = rgb_clamp((int) (green * brightnessValue));
            blue = rgb_clamp((int) (blue * brightnessValue));

            // set the new pixel back in
            line[w] =
                    ((red << 16) & 0x00FF0000) |
                    ((green << 8) & 0x0000FF00) |
                    (blue & 0x000000FF);
        }
        srcPixels = (char *) srcPixels + srcInfo.stride;
    }

    AndroidBitmap_unlockPixels(env, pSrcBitmap);
}

void Java_com_stetcho_basicimageprocessing_MainActivity_naSetGaussianBlur(JNIEnv *env,
                                                                         jclass clazz,
                                                                         jobject pSrcBitmap,
                                                                         jobject pOutBitmap) {
    float matrix[3][3] = {
            {0.0625, 0.125, 0.0625},
            {0.125,  0.25,  0.125},
            {0.0625, 0.125, 0.0625}
    };

    int srcResult, outResult, h, mh, w, mw;
    AndroidBitmapInfo srcInfo, outInfo;
    void *srcPixels, *outPixels, *tmpPixels, *malPixels;

    LOGI("Retrieve information about bitmap");

    if ((srcResult = AndroidBitmap_getInfo(env, pSrcBitmap, &srcInfo)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed! error = %d", srcResult);
        return;
    }

    if ((srcResult = AndroidBitmap_getInfo(env, pOutBitmap, &outInfo)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed! error = %d", srcResult);
        return;
    }

    if (srcInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888 ||
        outInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Invalid bitmap format. It should be RGBA");
        return;
    }

    if ((srcResult = AndroidBitmap_lockPixels(env, pSrcBitmap, &srcPixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed! error = %d", srcResult);
    }

    if ((outResult = AndroidBitmap_lockPixels(env, pOutBitmap, &outPixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed! error = %d", outResult);
    }

    malPixels = malloc(sizeof(unsigned char) * 4 * srcInfo.width * srcInfo.height);
    tmpPixels = malPixels;

    int i;
    for (i = 0; i < 10; i++) {
        for (h = 0; h < srcInfo.height; ++h) {
            for (w = 0; w < srcInfo.width; ++w) {
                unsigned char outRed = 0, outGreen = 0, outBlue = 0;
                unsigned char srcRed = 0, srcGreen = 0, srcBlue = 0;
                unsigned char *srcPixelsBuf = (unsigned char *) srcPixels;
                unsigned char *outPixelsBuf = (unsigned char *) outPixels;

                for (mh = 0; mh < 3; mh++) {
                    for (mw = 0; mw < 3; mw++) {
                        int coordX = w + mw - 1;
                        int coordY = h + mh - 1;
                        float factor = matrix[mh][mw];

                        if (coordX < 0) {
                            coordX = 0;
                        }

                        if (coordX >= srcInfo.width) {
                            coordX = srcInfo.width - 1;
                        }

                        if (coordY < 0) {
                            coordY = 0;
                        }

                        if (coordY >= srcInfo.height) {
                            coordY = srcInfo.height - 1;
                        }


                        int height_offset = (coordY * srcInfo.stride);
                        int width_offset = (coordX * 4);

                        srcRed = *(srcPixelsBuf + height_offset + width_offset);
                        srcGreen = *(srcPixelsBuf + height_offset + width_offset + 1);
                        srcBlue = *(srcPixelsBuf + height_offset + width_offset + 2);

                        outRed += srcRed * factor;
                        outGreen += srcGreen * factor;
                        outBlue += srcBlue * factor;
                    }
                }

                *(outPixelsBuf + h * srcInfo.stride + w * 4) = outRed;
                *(outPixelsBuf + h * srcInfo.stride + w * 4 + 1) = outGreen;
                *(outPixelsBuf + h * srcInfo.stride + w * 4 + 2) = outBlue;
            }
        }

        if (i == 0) {
            srcPixels = tmpPixels;
        }

        tmpPixels = srcPixels;
        srcPixels = outPixels;
        outPixels = tmpPixels;
    }

    AndroidBitmap_unlockPixels(env, pSrcBitmap);
    AndroidBitmap_unlockPixels(env, pOutBitmap);
    free(malPixels);
}

void Java_com_stetcho_basicimageprocessing_MainActivity_naRemoveBlueColor(JNIEnv *env,
                                                                         jclass clazz,
                                                                         jobject pSrcBitmap) {
    int srcResult, h, w;
    AndroidBitmapInfo srcInfo;
    void *srcPixels;

    LOGI("Retrieve information about bitmap");

    if ((srcResult = AndroidBitmap_getInfo(env, pSrcBitmap, &srcInfo)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed! error = %d", srcResult);
        return;
    }

    if (srcInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Invalid bitmap format. It should be RGBA");
        return;
    }

    if ((srcResult = AndroidBitmap_lockPixels(env, pSrcBitmap, &srcPixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed! error = %d", srcResult);
    }

    unsigned char *pixelBuf = (unsigned char *) srcPixels;

    for (h = 0; h < srcInfo.height; ++h) {
        for (w = 0; w < srcInfo.width; ++w) {
            unsigned char *pixelP = pixelBuf + h * srcInfo.stride + w * 4;

            // RED
//            *pixelP = (unsigned char) 0x00;

            // GREEN
//            *(pixelP+1) = (unsigned char)0x00;

            // BLUE
            *(pixelP + 2) = (unsigned char) 0x00;

#if VERBOSE_LOGGING
            LOGV("%d:%d:%d:%d", *pixelP, *(pixelP+1), *(pixelP+2), *(pixelP+3));
#endif
        }
    }
    AndroidBitmap_unlockPixels(env, pSrcBitmap);
}

}