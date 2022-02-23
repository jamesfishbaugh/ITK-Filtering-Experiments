# ITK-Filtering-Experiments

This repo contains experiments relating to image processing and filtering in ITK. 

A 2D RGB image serves as input, which is converted to grayscale and corruped with noise via an `itkAdditiveGaussianNoiseImageFilter`. The figure below shows the original and noisy image with a standard deviation of 50.

<p align="center">
  <img src="https://github.com/jamesfishbaugh/ITK-Filtering-Experiments/blob/main/data/figures/original_and_noisy.png" width="700" >
</p>

## Denoising with a Gaussian Filter

The noisy image is denoised with several values of **σ**.

<p align="center">
  <img src="https://github.com/jamesfishbaugh/ITK-Filtering-Experiments/blob/main/data/figures/gaussian_results.png" width="1000" >
</p>

## Denoising with a Median Filter

The noisy image is denoised with a median filter with several neighborhood sizes.

<p align="center">
  <img src="https://github.com/jamesfishbaugh/ITK-Filtering-Experiments/blob/main/data/figures/median_results.png" width="1000" >
</p>
