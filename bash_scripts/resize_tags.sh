#!/bin/bash

# ==============================================
# User inputs (change these settings!)
# ==============================================

# Set the folder to use
f=../data/apriltags/*.png  # Set the folder to convert from

# Set the desired width of your tags
width=60.0         # [mm]  Set the desired width in real units
dpi=300            # [ppi] Set the dpi (to control the print quality)
extra_margin=1     # [px]  Add an extra margin around your tags
default_width=10.0 # [px]  The pixel width of the original images

# Set the grid size you want to make
grid_h=1           # [num] Number of rows
grid_w=1           # [num] Number of columns
grid_filetype=pdf  # [png, jpg, pdf] Set the filetype for the grids

# Decide whether to keep intermediate files
keep_single_tag_images=true

# ==============================================
# Do some magick! (ImageMagick)
# ==============================================

# Calculate the scale factors
scale1=$(awk "BEGIN {printf \"%.2f\",${width}*(1+(2/${default_width})) * ${dpi} / 24.5 * 100 / ${default_width}"})
scale=$(awk "BEGIN {printf \"%d\", ${scale1}"})

pixels1=$(awk "BEGIN {printf \"%.2f\",${width}*(1+(2/${default_width})) * ${dpi} / 24.5"})
pixels=$(awk "BEGIN {printf \"%d\", ${pixels1}"})

margin=$(awk "BEGIN {printf \"%d\", ${pixels1}* ${extra_margin}/${default_width}"})

grid_check=$((grid_h * grid_w))

# Make a fresh output directory
OUTFOLDER="/home/carson/libs/ino_scripts/data/apriltags/output"

# Generate the output filename for the current image
ext="pdf"
echo $ext
outname="${OUTFOLDER}/output.${ext}"

# Rescale the current image
convert $f -scale $scale $outname
