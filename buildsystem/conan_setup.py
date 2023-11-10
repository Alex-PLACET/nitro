import argparse
import os
import platform
import subprocess
from pathlib import Path
from typing import Optional, Final
import re

def run_conan_install(conanfile : Path,
    compiler: str,
    compiler_version: str,
    cppstd: str,
    architecture : str,
    build_type : str,
    output_folder : Path,
    cmake_toolchain_generator: Optional[str] = None,
    libcxx: Optional[str] = None):
    cmd = [
        "conan",
        "install",
        conanfile,
        "-s", f"compiler={compiler}",
        "-s", f"arch={architecture}",
        "-s", f"compiler.cppstd={cppstd}",
        "-s", f"compiler.version={compiler_version}",
        "-s", f"build_type={build_type}",
        "-s", "compiler.runtime_type="+ "Debug" if build_type == "Debug" else "Release",
        "-of", f"{output_folder}",
        "-b", "missing",
        "-g", "CMakeDeps",
        "-g", "CMakeToolchain",
        "-g", "VirtualBuildEnv",
        "-g", "VirtualRunEnv",
    ]
    if cmake_toolchain_generator is not None:
        cmd.append("-c:b")
        cmd.append(f"tools.cmake.cmaketoolchain:generator={cmake_toolchain_generator}")
    if cmake_toolchain_generator is not None:
        cmd.append("-c:h")
        cmd.append(f"tools.cmake.cmaketoolchain:generator={cmake_toolchain_generator}")
    if libcxx is not None:
        cmd.append("-s")
        cmd.append(f"compiler.libcxx={libcxx}")
        
    subprocess.run(cmd, check=True)

class Compiler_information:
    def __init__(self, 
                 compiler_name : str, 
                 version : str,
                 architecture : str):
        self.compiler_name = compiler_name
        self.version = version
        self.architecture = architecture

def find_visual_studio_version(build_kit : str) -> Optional[str]:
    match = re.search(r'Visual Studio.*?(\d{4})', build_kit)
    if match:
        return match.group(1)
    else:
        return None
    
def visual_studio_version_to_msvc_version(version : str) -> Optional[str]:
    if version == "2022":
        return "193"
    elif version == "2019":
        return "192"
    elif version == "2017":
        return "191"
    elif version == "2015":
        return "190"
    elif version == "2013":
        return "180"
    return None

def find_msvc_architecture(build_kit : str) -> Optional[str]:
    return build_kit.split()[-1]

def build_kit_to_compiler_information(build_kit : str) -> Compiler_information:
    if "Visual Studio" in build_kit:
        compiler_name = "msvc"
        visual_studio_version = find_visual_studio_version(build_kit)
        if visual_studio_version is None:
            raise Exception("Visual Studio version not found")
        version = visual_studio_version_to_msvc_version(visual_studio_version)
        if version is None:
            raise Exception("Unknown Visual Studio version")
        architecture = find_msvc_architecture(build_kit)
        if architecture is None:
            architecture = "x64"
        elif architecture == "amd64":
            architecture = "x86_64"
        else:
            raise Exception("Unknown architecture for MSVC")
        return Compiler_information(compiler_name, version, architecture)
    

def parse_virtualenv_to_dotenv(directory : Path,
                               build_type:str,
                               architecture:str) -> Path:
    conanrun_extension : Final[str] = ".bat" if platform.system() == "Windows" else ".sh"
    conanrun_file_path : Final[Path] = directory / "build/" / build_type / "generators"/ ("conanrunenv-" + build_type.lower() + "-" + architecture + conanrun_extension)

    if not os.path.exists(conanrun_file_path):
        raise Exception(f"Virtual environment activation script not found on path: {conanrun_file_path}")

    with open(conanrun_file_path, "r") as f:
        lines = f.readlines()

    for line in lines:
        if line.startswith("set \"PATH="):
            path = line[10:].strip()
            break

    path = path.replace("\\", "/")
    path = path.replace("\"", "")
    path = path.split(";")

    dotenv_path : Final[Path] = directory.parent / "conan_run_env.env"
    to_write : Final[str] = "PATH=" + ";".join(path) + "\n"
    with open(dotenv_path, "w") as f:
        f.write(to_write)

    return dotenv_path

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate .env file using Conan for CMake and VirtualRunEnv")
    
    parser.add_argument("--conanfile",          type=str, required=True,    help="Path to the conanfile.py or conanfile.txt")
    parser.add_argument("--build_kit",          type=str, required=True,    help="CMake build_kit")
    parser.add_argument("--cppstd",             type=str, required=True,    help="C++ standard")
    parser.add_argument("--build_type",         type=str, required=True,    help="Build type (e.g. Debug, Release)")
    parser.add_argument("--output_directory",   type=str, required=True,    help="Output directory")
    parser.add_argument("--generator",          type=str, required=False,   help="Generator (e.g. Ninja, Makefile)")
    parser.add_argument("--libcxx",             type=str, required=False,   help="libcxx")
    
    args = parser.parse_args()

    # Parse build_kit to compiler information
    compiler_information = build_kit_to_compiler_information(args.build_kit)

    args.conanfile = Path(args.conanfile.replace('"', ''))
    args.output_directory = Path(args.output_directory.replace('"', ''))

    # Run Conan install command
    run_conan_install(
        args.conanfile,
        compiler_information.compiler_name,
        compiler_information.version,
        args.cppstd,
        compiler_information.architecture,
        args.build_type,
        args.output_directory,
        args.generator,
        args.libcxx)
    
    # Parse virtual environment activation script to .env file
    dotenv_path = parse_virtualenv_to_dotenv(args.output_directory, args.build_type, compiler_information.architecture)


    print(f"✅ .env file has been generated in {dotenv_path}")
