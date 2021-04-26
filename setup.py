from setuptools import setup

setup(
    name='mp3-tagging',
    version='1.0',
    description='Personal script to automatically tag mp3 files.',
    license="GPL-3.0",
    author='jptrzy',
    author_email='jptrzy@gmail.com',
    #packages=['mp3-tagging'],  #same as name
    install_requires=["mutagen"], #external packages as dependencies
    scripts=[
        "mp3-tagging"
    ]
)

