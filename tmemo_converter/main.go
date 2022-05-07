package main

import (
	"fmt"
	"image"
	"image/color"
	"image/png"
	"io/ioutil"
	"os"
)

const (
	bytesPerPixel = 2
	width         = 480
	height        = 320
	toolbarHeight = 16
	actualHeight  = height - toolbarHeight

	total = width * actualHeight * bytesPerPixel
)

func main() {
	if len(os.Args) == 1 {
		fmt.Printf("Usage: %s INPUT ...\n", os.Args[0])
		fmt.Printf("Example: %s tmemo000.tmd tmemo001.tmd\n", os.Args[0])
		os.Exit(0)
	}

	for _, a := range os.Args[1:] {
		err := convert(a)
		if err != nil {
			fmt.Fprintf(os.Stderr, "failed to convert %s: %s\n", a, err)
			os.Exit(1)
		}
	}
}

func convert(fn string) error {
	raw, err := ioutil.ReadFile(fn)
	if err != nil {
		return fmt.Errorf("failed to read %s: %s", fn, err)
	}

	if len(raw) != total {
		return fmt.Errorf("unexpected binary length: expect=%d, actual=%d", total, len(raw))
	}

	rgba := image.NewRGBA(image.Rect(0, 0, width, actualHeight))

	for i := 0; i < width*actualHeight; i++ {
		pixel := uint16(raw[i*2+1])<<8 + uint16(raw[i*2])
		r := float64(pixel&0xf800>>11) * 255 / 31
		g := float64(pixel&0x07e0>>5) * 255 / 63
		b := float64(pixel&0x001f) * 255 / 31
		rgba.SetRGBA(i%width, actualHeight-i/width, color.RGBA{R: uint8(r), G: uint8(g), B: uint8(b), A: 255})
	}

	newFn := fn + ".png"
	out, err := os.Create(newFn)
	if err != nil {
		return fmt.Errorf("failed to create a file %s: %s", newFn, err)
	}
	defer out.Close()

	err = png.Encode(out, rgba)
	if err != nil {
		return fmt.Errorf("failed to compress the image into PNG: %s", err)
	}

	return nil
}
