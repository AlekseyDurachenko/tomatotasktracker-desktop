#!/bin/bash

mkdir -p ../res/icons/hicolor/1024x1024
mkdir -p ../res/icons/hicolor/512x512
mkdir -p ../res/icons/hicolor/128x128
mkdir -p ../res/icons/hicolor/64x64
mkdir -p ../res/icons/hicolor/32x32

xcf2png xcf/tomatotasktracker-desktop.xcf -o ../res/icons/hicolor/1024x1024/tomatotasktracker-desktop.png
convert -resize 512x512 ../res/icons/hicolor/1024x1024/tomatotasktracker-desktop.png ../res/icons/hicolor/512x512/tomatotasktracker-desktop.png
convert -resize 128x128 ../res/icons/hicolor/1024x1024/tomatotasktracker-desktop.png ../res/icons/hicolor/128x128/tomatotasktracker-desktop.png
convert -resize 64x64 ../res/icons/hicolor/1024x1024/tomatotasktracker-desktop.png ../res/icons/hicolor/64x64/tomatotasktracker-desktop.png
convert -resize 32x32 ../res/icons/hicolor/1024x1024/tomatotasktracker-desktop.png ../res/icons/hicolor/32x32/tomatotasktracker-desktop.png
convert -resize 128x128 ../res/icons/hicolor/1024x1024/tomatotasktracker-desktop.png ../res/windows/tomatotasktracker-desktop.ico

cd xcf
for i in *.xcf; do
  echo $i;
  xcf2png $i -o ../../res/icons/hicolor/128x128/${i%%.*}.png
  convert -resize 128x128 ../../res/icons/hicolor/128x128/${i%%.*}.png ../../res/icons/hicolor/128x128/${i%%.*}.png
done;
cd ..
