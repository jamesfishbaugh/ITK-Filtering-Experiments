#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRGBToLuminanceImageFilter.h"
#include <itkAdditiveGaussianNoiseImageFilter.h>
#include <itkDiscreteGaussianImageFilter.h>

int main(int argc, char ** argv)
{
  
  if (argc < 2)
  {
    std::cout << " USAGE: FilteringExperiments input_image" << std::endl;
  }
  
  // The file path for the image to experiment with
  char* imPath = argv[1];
  
  // This application assumes 2 dimensional RGB images 
  constexpr unsigned int Dimension = 2;
  using ComponentType = unsigned char;
  using InputPixelType = itk::RGBPixel<ComponentType>;
  using InputImageType = itk::Image<InputPixelType, Dimension>;
  
  // Read the image
  using ReaderType = itk::ImageFileReader<InputImageType>;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(imPath);
  
  // Setup the output image type
  using OutputPixelType = unsigned char;
  using GrayScaleImageType = itk::Image<OutputPixelType, Dimension>;
  
  // Convert the image to grayscale
  using FilterType = itk::RGBToLuminanceImageFilter<InputImageType, GrayScaleImageType>;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  filter->Update();
  
  // Store the grayscale image  
  GrayScaleImageType::Pointer grayscaleImage = filter->GetOutput();
  
  // Add noise via to the grayscale image via the AdditiveGaussianNoiseImageFilter
  using NoiseFilterType = itk::AdditiveGaussianNoiseImageFilter<GrayScaleImageType, GrayScaleImageType>;
  NoiseFilterType::Pointer noiseFilter = NoiseFilterType::New();
  noiseFilter->SetStandardDeviation(50.0);
  noiseFilter->SetInput(grayscaleImage);
  noiseFilter->Update();
  
  // Store the noisy image
  GrayScaleImageType::Pointer noisyImage = noiseFilter->GetOutput();
  
  // Write the noisy image
  using WriterType = itk::ImageFileWriter<GrayScaleImageType>;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("noisy.png");
  writer->SetInput(noisyImage);

  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject & error)
  {
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
  }

  // Attempt to denoise via a Gaussian filter
  using GaussianImageFilterType = itk::DiscreteGaussianImageFilter<GrayScaleImageType, GrayScaleImageType>;
  GaussianImageFilterType::Pointer gaussianDenoiseFilter = GaussianImageFilterType::New();
  
  // Let's try some different sigmas
  std::vector<float> sigmas = {0.25, 1.0, 3.0, 10.0};
  
  for (int i=0; i<sigmas.size(); i++)
  {
    
    gaussianDenoiseFilter->SetSigma(sigmas[i]);
    gaussianDenoiseFilter->SetInput(noisyImage);
    gaussianDenoiseFilter->Update();
    
    // Store the Gaussian denoised image
    GrayScaleImageType::Pointer gaussianDenoisedImage = gaussianDenoiseFilter->GetOutput();
    
    // Write the Gaussian denoised image
    char buffer [50];
    sprintf(buffer, "gaussian_denoised_sigma_%0.2f.png", sigmas[i]);
    writer->SetFileName(buffer);
    writer->SetInput(gaussianDenoisedImage);

    try
    {
      writer->Update();
    }
    catch (itk::ExceptionObject & error)
    {
      std::cerr << "Error: " << error << std::endl;
      return EXIT_FAILURE;
    }
  
  }
    
  return EXIT_SUCCESS;
}
