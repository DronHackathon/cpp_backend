from setuptools import setup, Extension
setup(name='force_api',
      version='0.6',
      description='Implementation of libforce in python',
      author='Alex Nesterenko',
      author_email='nestorf250@gmail.com',
      ext_modules=[Extension('force_api',
      							['force_api.c'],
      							libraries=['serialport','force'],
                                  extra_compile_args=['-std=c11']
      							)],
      py_modules=['force_api']
)
