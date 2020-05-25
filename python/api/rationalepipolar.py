#  -----------------------------------------------------------------------------
#   Copyright (c) 2020.  All rights reserved.
#  -----------------------------------------------------------------------------

import argparse
import expyext as epy

def genRationalEpi(leftimg, leftmdl, rightimg, rightmdl, bands, sample, outdir, resample, memsize):
  epy.generateRationalEpipolar(leftimg, leftmdl, rightimg, rightmdl, bands, sample, outdir, resample, memsize)

'''
  The script rationalepipolar generates pseudo epipolar images 
  from a pair of overlapping images represented by rational function models.

  usage:  rationalepipolar.py 
            --leftimg <path_of_left_image_file>    // 'images/img01.tif'
            --leftmdl <path_of_left_model_file>    // 'images/img01_rpc.tif'
            --rightimg <path_of_right_image_file>  // 'images/img02.tif'
            --rightmdl <path_of_right_model_file>  // 'images/img02_rpc.tif'
            --bands [list_of_band_ids]             // [1,2,3]
            --sample [downsample_factor]           // 2
            --outdir [path_of_output_directory]    // 'd:/out/'
            --resample [cubic|gaussian|sinc]
            --memsize [memory_size_in_MB]          // 4000
            
  example:
  
  python D:/home/workspace/install/windows/py/api/rationalepipolar.py --leftimg "D:/home/workspace/TestData/Imagery/Satellite/po_162796_pan/po_162796_pan_0000000.tif" --leftmdl "D:/home/workspace/TestData/Imagery/Satellite/po_162796_pan/po_162796_pan_0000000_rpc.txt" --rightimg "D:/home/workspace/TestData/Imagery/Satellite/po_162796_pan/po_162796_pan_0010000.tif" --rightmdl "D:/home/workspace/TestData/Imagery/Satellite/po_162796_pan/po_162796_pan_0010000_rpc.txt" --bands 1 --sample 1 --outdir "D:/home/workspace/build/windows/vs2019/tests/output/" --resample "cubic" --memsize 4000
'''

if __name__ == '__main__':
  ap = argparse.ArgumentParser(description='generates epipolar images')
  
  ap.add_argument('-i', '--leftimg', type=str, required=True,
    help='path of left image file')
  ap.add_argument('-m', '--leftmdl', type=str, required=True,
    help='path of left model file')
  ap.add_argument('-j', '--rightimg', type=str, required=True,
    help='path of right image file')
  ap.add_argument('-n', '--rightmdl', type=str, required=True,
    help='path of right model file')
  ap.add_argument('-b', '--bands', nargs='*', default=[], type=int,
    help='list of band ids')
  ap.add_argument('-s', '--sample', default=1, type=int,
    help='downsample factor')
  ap.add_argument('-o', '--outdir', type=str, required=True,
    help='output path to the generated epipolar images')
  ap.add_argument('-r', '--resample', default='cubic', type=str, choices=['bilinear', 'cubic', 'gaussian'],
    help='bilinear|cubic|gaussian')
  ap.add_argument('--memsize', default=4000, type=int,
    help='memory size in MB')
  
  args = vars(ap.parse_args())
  #print(args)
  
  leftimg = args['leftimg']
  leftmdl = args['leftmdl']
  rightimg = args['rightimg']
  rightmdl = args['rightmdl']
  bands = args['bands']
  sample = args['sample']
  outdir = args['outdir']
  resample = args['resample']
  memsize = args['memsize']

  genRationalEpi(leftimg, leftmdl, rightimg, rightmdl, bands, sample, outdir, resample, memsize)
  