sed -e 's/\x01/\x44\x66\xed/g;s/\x00/\xf9\xfc\x10/g' $1.data > $1.rgb
convert -size 1920X1080 -depth 8 rgb:$1.rgb $1.png
