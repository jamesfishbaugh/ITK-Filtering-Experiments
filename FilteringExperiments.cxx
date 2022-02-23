#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRGBToLuminanceImageFilter.h"
#include <itkAdditiveGaussianNoiseImageFilter.h>

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
  
  GrayScaleImageType::Pointer noisyImage = noiseFilter->GetOutput();

  // Write the grayscale image
  using WriterType = itk::ImageFileWriter<GrayScaleImageType>;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("test.png");
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

  return EXIT_SUCCESS;
}
