#  -----------------------------------------------------------------------------
#   Copyright (c) 2019.  All rights reserved.
#  -----------------------------------------------------------------------------

import argparse
from api import ExApi


def main() -> int:
  ap = argparse.ArgumentParser()
  
  ap.add_argument('-t', '--sensorname', type=str, choices=['frame', 'rational'], required=True,
    help='sensor name: frame|rational')
  ap.add_argument('-p', '--prjdir', type=str, required=False,
    help='path to the project directory or file')
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
    help='resample kernel: bilinear|cubic|gaussian')
  ap.add_argument('--memsize', default=4000, type=int,
    help='memory size in MB')

  args = vars(ap.parse_args())
  st = args['sensorname']
  prjdir = args['prjdir']
  leftimg = args['leftimg']
  leftmdl = args['leftmdl']
  rightimg = args['rightimg']
  rightmdl = args['rightmdl']
  bands = args['bands']
  sample = args['sample']
  outdir = args['outdir']
  resample = args['resample']
  memsize = args['memsize']

  return 0
  
if __name__ == '__main__':
  """The script epipolar generates epipolar or pseudo epipolar images 
    from a pair of overlapping images acquired by the same sensor.

    \detail Epipolar geometry is from the geometry of stereo vision. When a camera
    views a scene from two or more distinct positions, there are a number of 
    geometric relations between the 3D surface and its projections onto the 
    2D images that lead to constraints between the image pixels.

    usage:  epipolar.py 
              --sensorname <frame|rational>
              --prjdir [path_of_project_directory_or_file]
              --leftimg <path_of_left_image_file>    // 'images/img01.tif'
              --leftmdl <path_of_left_model_file>    // 'images/img01_rpc.tif'
              --rightimg <path_of_right_image_file>  // 'images/img02.tif'
              --rightmdl <path_of_right_model_file>  // 'images/img02_rpc.tif'
              --bands [list_of_band_ids]             // [1,2,3]
              --sample [downsample_factor]           // 2
              --outdir [path_of_output_directory]    // 'd:/out/'
              --resample [cubic|gaussian|sinc]
              --memsize [memory_size_in_MB]          // 4000
  """
  sys.exit(main())
