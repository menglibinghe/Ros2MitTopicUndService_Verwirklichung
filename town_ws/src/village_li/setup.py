from setuptools import find_packages, setup

package_name = 'village_li'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='lubuntu',
    maintainer_email='menglibinghe@163.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            "li4_node = village_li.li4:main",       #new added content,li4_node is the executable file name
            "li3_node = village_li.li3:main"        #new added content
        ],
    },
)
