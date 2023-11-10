from conan import ConanFile
from conan.tools.cmake import cmake_layout

class Nitro(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "VirtualRunEnv"

    def requirements(self):
        self.requires("opencv/4.5.5")
        self.requires("zlib/1.3")
        self.requires("brotli/1.1.0", override=True)

    def config_options(self):
        self.options["opencv"].shared=True
        self.options["opencv"].fPIC=True
        self.options["opencv"].parallel="tbb"
        self.options["opencv"].with_ipp="opencv-icv"
        self.options["opencv"].with_eigen=True
        self.options["opencv"].neon=False
        self.options["opencv"].with_opencl=True
        self.options["opencv"].with_cuda=False
        self.options["opencv"].with_cublas=False
        self.options["opencv"].with_cufft=False
        self.options["opencv"].with_cudnn=False
        self.options["opencv"].cuda_arch_bin=None
        self.options["opencv"].cpu_baseline="AVX"
        self.options["opencv"].cpu_dispatch="AVX2"
        self.options["opencv"].world=False
        self.options["opencv"].nonfree=False
        self.options["opencv"].with_vulkan=True
        self.options["opencv"].dnn_cuda=False
        self.options["opencv"].with_gtk=False
        self.options["opencv"].with_qt=False
        self.options["opencv"].with_jpeg="libjpeg-turbo"
        self.options["opencv"].with_png=True
        self.options["opencv"].with_tiff=True
        self.options["opencv"].with_jpeg2000="jasper"
        self.options["opencv"].with_openexr=True
        self.options["opencv"].with_webp=True
        self.options["opencv"].with_gdal=False
        self.options["opencv"].with_gdcm=False
        self.options["opencv"].with_imgcodec_hdr=False
        self.options["opencv"].with_imgcodec_pfm=False
        self.options["opencv"].with_imgcodec_pxm=False
        self.options["opencv"].with_imgcodec_sunraster=False
        self.options["opencv"].with_msmf=True
        self.options["opencv"].with_msmf_dxva=True
        self.options["opencv"].with_quirc=True
        self.options["opencv"].with_ffmpeg=True
        self.options["opencv"].with_v4l=False
        self.options["opencv"].with_tesseract=True


    def layout(self):
        cmake_layout(self)
