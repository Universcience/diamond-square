#version 3.6;

global_settings{ assumed_gamma 2.0 }

camera {
	location <0,40,-50>
	look_at  <0,0,0>
}

sky_sphere{ pigment{ color<0.4,0.4,0.8> } }

light_source{ <200,200,-200>, color rgb 1.0 }

height_field {
	"data.pgm"
//	smooth
	scale <100,20,100>
	translate <-50,0,-50>
	texture{ pigment{ color<0.6,0.6,0.6> } }
}
