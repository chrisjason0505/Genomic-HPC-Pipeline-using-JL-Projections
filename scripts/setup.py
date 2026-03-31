from setuptools import setup, Extension

ext = Extension(
    name="encode_utils_compiled",
    sources=["encode_utils.c"],
    # Do not link against python libs or we'll make a true python extension
    # Actually setup will always make a python extension. Let's provide a dummy init function in C.
)

setup(name="encode_utils_compiled", ext_modules=[ext])
