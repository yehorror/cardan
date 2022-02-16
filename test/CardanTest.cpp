#include <gtest/gtest.h>
#include <v8.h>
#include <libplatform/libplatform.h>

int main(int argc, char** argv)
{
    auto platformUnique = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platformUnique.get());
    v8::V8::Initialize();

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
