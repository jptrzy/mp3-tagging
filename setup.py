from setuptools import setup

import os
import shutil
import mp3_tagging
from hashlib import sha512

SCRIPTS_PATH = 'build_scripts'

def hash_file(path):
    with open(path, 'rb') as fobj:
        return sha512(fobj.read()).digest()


def scripts_hack(*scripts):
    ''' Hack around `pip install` temporary directories '''
    if not os.path.exists(SCRIPTS_PATH):
        os.makedirs(SCRIPTS_PATH)
    scripts_path = []
    for src_path, basename in scripts:
        dest_path = os.path.join(SCRIPTS_PATH, basename)
        if not os.path.exists(dest_path) or \
                (os.path.exists(src_path) and hash_file(src_path) != hash_file(dest_path)):
            shutil.copy(src_path, dest_path)
        scripts_path += [dest_path]
    return scripts_path

setup(
    name='mp3-tagging',
    version=mp3_tagging.__version__,
    description='Personal script to automatically tag mp3 files.',
    license=mp3_tagging.__license__,
    author=mp3_tagging.__author__,
    author_email=mp3_tagging.__email__,
    keywords = "mp3 music",
    classifiers=[
        'Environment :: Console',
        'License :: OSI Approved :: GNU General Public License v3 (GPLv3)',
        'Operating System :: POSIX',
        'Operating System :: Unix',
    ],
    packages=['mp3_tagging'],  #same as name
    install_requires=["mutagen"], #external packages as dependencies
    scripts=scripts_hack(
        ("mp3-tagging.py", "mp3-tagging")
    )
)

# setup(
#     name = "an_example_pypi_project",
#     version = "0.0.4",
#     author = "Andrew Carter",
#     author_email = "andrewjcarter@gmail.com",
#     description = ("An demonstration of how to create, document, and publish "
#                                    "to the cheese shop a5 pypi.org."),
#     license = "BSD",
#     keywords = "example documentation tutorial",
#     url = "http://packages.python.org/an_example_pypi_project",
#     packages=['an_example_pypi_project', 'tests'],
#     long_description=read('README'),
#     classifiers=[
#         "Development Status :: 3 - Alpha",
#         "Topic :: Utilities",
#         "License :: OSI Approved :: BSD License",
#     ],
# )


