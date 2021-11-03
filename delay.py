# This script is needed for running 'platformio test'.
# There is a bug with the platformio teensy integration: 
#     https://community.platformio.org/t/teensy-4-1-unit-testing-issue/21033/5
# Hopefully solved soon:
#     https://github.com/platformio/platformio-core/issues/3742
Import("env")

def after_upload(source, target, env):
    print("Delay while uploading...")
    import time
    time.sleep(5)
    print("Done!")

env.AddPostAction("upload", after_upload)

