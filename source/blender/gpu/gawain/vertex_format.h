
// Gawain vertex format
//
// This code is part of the Gawain library, with modifications
// specific to integration with Blender.
//
// Copyright 2016 Mike Erwin
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of
// the MPL was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "common.h"

#define MAX_VERTEX_ATTRIBS 16
#define AVG_VERTEX_ATTRIB_NAME_LEN 5
#define VERTEX_ATTRIB_NAMES_BUFFER_LEN ((AVG_VERTEX_ATTRIB_NAME_LEN + 1) * MAX_VERTEX_ATTRIBS)

#define USE_10_10_10 0
// (GLEW_VERSION_3_3 || GLEW_ARB_vertex_type_2_10_10_10_rev)
//   ^-- this is only guaranteed on Windows right now, will be true on all platforms soon

typedef enum {
	COMP_I8 = GL_BYTE,
	COMP_U8 = GL_UNSIGNED_BYTE,
	COMP_I16 = GL_SHORT,
	COMP_U16 = GL_UNSIGNED_SHORT,
	COMP_I32 = GL_INT,
	COMP_U32 = GL_UNSIGNED_INT,

	COMP_F32 = GL_FLOAT, // TODO: drop the GL_ equivalence here, use a private lookup table

#if USE_10_10_10
	COMP_I10 = GL_INT_2_10_10_10_REV
#endif
} VertexCompType;

typedef enum {
	KEEP_FLOAT,
	KEEP_INT,
	NORMALIZE_INT_TO_FLOAT, // 127 (ubyte) -> 0.5 (and so on for other int types)
	CONVERT_INT_TO_FLOAT // 127 (any int type) -> 127.0
} VertexFetchMode;

typedef struct {
	VertexCompType comp_type;
	unsigned comp_ct; // 1 to 4
	unsigned sz; // size in bytes, 1 to 16
	unsigned offset; // from beginning of vertex, in bytes
	VertexFetchMode fetch_mode;
	const char* name;
} Attrib;

typedef struct {
	unsigned attrib_ct; // 0 to 16 (MAX_VERTEX_ATTRIBS)
	unsigned stride; // stride in bytes, 1 to 256
	bool packed;
	Attrib attribs[MAX_VERTEX_ATTRIBS]; // TODO: variable-size attribs array
	char names[VERTEX_ATTRIB_NAMES_BUFFER_LEN];
	unsigned name_offset;
} VertexFormat;

void VertexFormat_clear(VertexFormat*);
void VertexFormat_copy(VertexFormat* dest, const VertexFormat* src);

unsigned add_attrib(VertexFormat*, const char* name, VertexCompType, unsigned comp_ct, VertexFetchMode);

// for internal use
void VertexFormat_pack(VertexFormat*);
unsigned padding(unsigned offset, unsigned alignment);
unsigned vertex_buffer_size(const VertexFormat*, unsigned vertex_ct);