unsigned char loader[] = {
  0x55, 0x89, 0xe5, 0x83, 0xec, 0x14, 0xc7, 0x45, 0xfc, 0x04, 0x00, 0x00,
  0x00, 0xeb, 0x1a, 0x8b, 0x45, 0xfc, 0x89, 0x04, 0x24, 0xe8, 0x6d, 0x00,
  0x00, 0x00, 0x83, 0xf8, 0x01, 0x74, 0x06, 0xb8, 0x01, 0x00, 0x00, 0x00,
  0x82, 0x83, 0x45, 0xfc, 0x02, 0x81, 0x7d, 0xfc, 0xe8, 0x03, 0x00, 0x00,
  0x7e, 0xdd, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x82, 0xb8, 0x00, 0x00, 0x00,
  0x00, 0xc9, 0xc3, 0x00, 0x55, 0x89, 0xe5, 0x83, 0xec, 0x10, 0x83, 0x7d,
  0x08, 0x01, 0x7f, 0x07, 0xb8, 0x00, 0x00, 0x00, 0x00, 0xeb, 0x32, 0xc7,
  0x45, 0xfc, 0x02, 0x00, 0x00, 0x00, 0xeb, 0x1c, 0x8b, 0x45, 0x08, 0x89,
  0xc2, 0xc1, 0xfa, 0x1f, 0xf7, 0x7d, 0xfc, 0x89, 0xd0, 0x85, 0xc0, 0x75,
  0x07, 0xb8, 0x00, 0x00, 0x00, 0x00, 0xeb, 0x11, 0x83, 0x45, 0xfc, 0x01,
  0x8b, 0x45, 0xfc, 0x3b, 0x45, 0x08, 0x7c, 0xdc, 0xb8, 0x01, 0x00, 0x00,
  0x00, 0xc9, 0xc3, 0x55, 0x89, 0xe5, 0x83, 0xec, 0x14, 0xc7, 0x45, 0xfc,
  0x02, 0x00, 0x00, 0x00, 0xeb, 0x32, 0x8b, 0x45, 0xfc, 0x89, 0x04, 0x24,
  0xe8, 0x9f, 0xff, 0xff, 0xff, 0x85, 0xc0, 0x74, 0x1f, 0x8b, 0x45, 0xfc,
  0x8b, 0x55, 0x08, 0x89, 0xd1, 0x29, 0xc1, 0x89, 0xc8, 0x89, 0x04, 0x24,
  0xe8, 0x87, 0xff, 0xff, 0xff, 0x85, 0xc0, 0x74, 0x07, 0xb8, 0x01, 0x00,
  0x00, 0x00, 0xeb, 0x11, 0x83, 0x45, 0xfc, 0x01, 0x8b, 0x45, 0xfc, 0x3b,
  0x45, 0x08, 0x7c, 0xc6, 0xb8, 0x00, 0x00, 0x00, 0x00, 0xc9, 0xc3, 0x00,
  0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x7a, 0x52, 0x00,
  0x01, 0x7c, 0x08, 0x01, 0x1b, 0x0c, 0x04, 0x04, 0x88, 0x01, 0x00, 0x00,
  0x1c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x48, 0xff, 0xff, 0xff,
  0x47, 0x00, 0x00, 0x00, 0x00, 0x41, 0x0e, 0x08, 0x85, 0x02, 0x42, 0x0d,
  0x05, 0x02, 0x43, 0xc5, 0x0c, 0x04, 0x04, 0x00, 0x1c, 0x00, 0x00, 0x00,
  0x3c, 0x00, 0x00, 0x00, 0x6f, 0xff, 0xff, 0xff, 0x50, 0x00, 0x00, 0x00,
  0x00, 0x41, 0x0e, 0x08, 0x85, 0x02, 0x42, 0x0d, 0x05, 0x02, 0x4c, 0xc5,
  0x0c, 0x04, 0x04, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x5c, 0x00, 0x00, 0x00,
  0xc8, 0xfe, 0xff, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x41, 0x0e, 0x08,
  0x85, 0x02, 0x42, 0x0d, 0x05, 0x7b, 0xc5, 0x0c, 0x04, 0x04, 0x00, 0x00
};
unsigned int loader_len = 336;
