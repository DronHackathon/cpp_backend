from setuptools import setup, Extension
setup(name='Force_client',
      version='0.1',
      description='Implementation of libforce in python',
      author='Alex Nesterenko',
      author_email='nestorf250@gmail.com',
      ext_modules=[Extension('force_client',
      							['client.c'],
      							libraries=['serialport','libforce']
      							)],
      py_modules=['force_client']
)
