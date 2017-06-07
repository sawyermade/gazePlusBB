# gazePlusBB

Tested on Ubuntu 16.04 LTS with OpenCV2 in /usr/include/opencv2

GATECH GAZE+ Data Set: http://ai.stanford.edu/~alireza/GTEA_Gaze_Website/GTEA_Gaze+.html

Compilation: Run make in directory

Usage: ./gazebb videoFileName.avi videoFileName.txt boxSize1D

Output: ./videoFileName.avi-BB.avi

Example: ./gazebb Alireza_Pizza2.avi Alireza_Pizza2.txt 250

Clean: make clean
