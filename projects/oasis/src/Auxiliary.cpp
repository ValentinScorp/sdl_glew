#include "Precompiled.h"

void aux::surface::create(Uint16 patch_w, Uint16 patch_h, Uint16 w, Uint16 h, float tileStep) {
    width = patch_w * w;
    height = patch_h * h;
    step = tileStep;
    
    for (Uint16 y = 0; y <= height; y++) {
        for (Uint16 x = 0; x <= width; x++) {
            vertex v(glm::fvec3(x * tileStep, y * tileStep, 0.0f), glm::fvec3(0.0f, 0.0f, 1.0f));
            vertices.push_back(v);
        }
    }
    
    for (Uint16 py = 0; py < h; py++) {
        for (Uint16 px = 0; px < w; px++) {
            
            glm::ivec2 ioffs = glm::ivec2(px * patch_w, py * patch_h);
            
            for (Uint16 y = 0; y < patch_h; y++) {
                for (Uint16 x = 0; x < patch_w; x++) {
                    size_t a_idx = (ioffs.x + x) + (y + ioffs.y) * (width + 1);
                    size_t b_idx = a_idx + 1;
                    size_t c_idx = b_idx + (width + 1);
                    size_t d_idx = a_idx + (width + 1);
                    
                    float w_step = 1.0f / patch_w;
                    float h_step = 1.0f / patch_h;
                    
                    glm::fvec2 a_tex0(w_step * x,        h_step * y);
                    glm::fvec2 b_tex0(w_step * (x + 1),  h_step * y);
                    glm::fvec2 c_tex0(w_step * (x + 1),  h_step * (y + 1));
                    glm::fvec2 d_tex0(w_step * x,        h_step * (y + 1));
                    
                    glm::fvec2 a_tex1(0.0f, 0.0f);
                    glm::fvec2 b_tex1(1.0f, 0.0f);
                    glm::fvec2 c_tex1(1.0f, 1.0f);
                    glm::fvec2 d_tex1(0.0f, 1.0f);
                    
                    triangle tri1(a_idx, b_idx, c_idx);
                    
                    vertices[a_idx].triangles.push_back(triangles.size());
                    vertices[b_idx].triangles.push_back(triangles.size());
                    vertices[c_idx].triangles.push_back(triangles.size());
                    
                    tri1.a_tex0 = a_tex0;
                    tri1.b_tex0 = b_tex0;
                    tri1.c_tex0 = c_tex0;
                    
                    tri1.a_tex1 = a_tex1;
                    tri1.b_tex1 = b_tex1;
                    tri1.c_tex1 = c_tex1;
                    
                    triangles.push_back(tri1);
                    
                    triangle tri2(a_idx, c_idx, d_idx);
                    
                    vertices[a_idx].triangles.push_back(triangles.size());
                    vertices[c_idx].triangles.push_back(triangles.size());
                    vertices[d_idx].triangles.push_back(triangles.size());
                    
                    tri2.a_tex0 = a_tex0;
                    tri2.b_tex0 = c_tex0;
                    tri2.c_tex0 = d_tex0;
                    
                    tri2.a_tex1 = a_tex1;
                    tri2.b_tex1 = c_tex1;
                    tri2.c_tex1 = d_tex1;
                    
                    triangles.push_back(tri2);
                }
            }
        }
    }
}

void aux::surface::createTriangle(size_t idx_a, size_t idx_b, size_t idx_c) {
    triangle tri(idx_a, idx_b, idx_c);
    vertices[idx_a].triangles.push_back(triangles.size());
    vertices[idx_b].triangles.push_back(triangles.size());
    vertices[idx_c].triangles.push_back(triangles.size());
    triangles.push_back(tri);
}

bool aux::surface::intersectRayTriangle(aux::ray ray, size_t tIdx, glm::fvec3 &intersectionVertex)
{
	// находим вектора сторон треугольника
    
	glm::fvec3 u = vertices[triangles[tIdx].b_idx].pos - vertices[triangles[tIdx].a_idx].pos;
	glm::fvec3 v = vertices[triangles[tIdx].c_idx].pos - vertices[triangles[tIdx].a_idx].pos;

	// находим нормаль к треугольнику
	glm::fvec3 n = glm::cross(u, v);

	if (n.x == 0 && n.y == 0 && n.z == 0) {
		return 0;                       // неверные параметры треугольника (либо точки на одной прямой, либо все в одной точке)
	}

	// вектор направления луча
	glm::fvec3 dir = ray.end - ray.begin;
	// векор от точки на треугольнике до начала луча
	glm::fvec3 w0 = ray.begin - vertices[triangles[tIdx].a_idx].pos;

    float a = -glm::dot(n, w0);
    float b = glm::dot(n, dir);

	if (fabs(b) < 0.0001) {       // луч паралельный плоскости треугольника
		if (a == 0) { return 0; }   // луч лежит на плоскости треугольника
		else { return 0; }   // луч не на плоскости треугольника
	}
	// найдем точку пересечения луча с треугольником
	float r = a / b;
	if (r < 0.0) {
		return 0;                    // луч идет мимо треугольника
	}
	// для сегмента проверить также (r > 1.0) => нет пересечения

	intersectionVertex = ray.begin + dir * r;           // точка пересечения луча и плоскости
														// лежит ли точка в треугольнике
       
	float    uu, uv, vv, wu, wv, D;
	uu = glm::dot(u, u);
	uv = glm::dot(u, v);
	vv = glm::dot(v, v);
	glm::fvec3 w = intersectionVertex - vertices[triangles[tIdx].a_idx].pos;
	wu = glm::dot(w, u);
	wv = glm::dot(w, v);
	D = uv * uv - uu * vv;

	// найдем и проверим параметрические координаты
	float s = (uv * wv - vv * wu) / D;
	if (s < 0.0 || s > 1.0) {
		return 0;                   // точка вне треугольника
	}
	float t = (uv * wu - uu * wv) / D;
	if (t < 0.0 || (s + t) > 1.0) {
		return 0;                   // точка вне треугольника
	}

	return 1;
}

size_t aux::surface::getClosestPoint(size_t tIdx, glm::fvec3 point) {
    glm::fvec3 a_vector = glm::abs(point - vertices[triangles[tIdx].a_idx].pos);
    glm::fvec3 b_vector = glm::abs(point - vertices[triangles[tIdx].b_idx].pos);
    glm::fvec3 c_vector = glm::abs(point - vertices[triangles[tIdx].c_idx].pos);
    
    float length_a = glm::length(a_vector);
    float length_b = glm::length(b_vector);
    float length_c = glm::length(c_vector);
    
    if (length_a < length_b) {
        if (length_a < length_c) {
            return triangles[tIdx].a_idx;
        } else {
            return triangles[tIdx].c_idx;
        }
    } else {
        if (length_b < length_c) {
            return triangles[tIdx].b_idx;
        } else {
            return triangles[tIdx].c_idx;
        }
    }
    return 0;
}

void aux::surface::recalcTriangleNormals() {
    for (auto& tri: triangles) {
        tri.calcNormal(vertices);
    }
}

void aux::surface::recalcVertexNormals() {
    for (auto &v : vertices) {
        glm::fvec3 normal = { 0.0f, 0.0f, 0.0f };
        for (auto &t: v.triangles) {
            normal += triangles[t].nor;
        }
        normal /= v.triangles.size();
        normal = glm::normalize(normal);
        v.nor = normal;
    }
}

float aux::surface::sign(glm::fvec2 p1, glm::fvec2 p2, glm::fvec2 p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool aux::surface::isPointOnTriangle(glm::fvec2 point, size_t tIdx) {
    float d1, d2, d3;
    bool has_neg, has_pos;
    
    glm::fvec2 a (vertices[triangles[tIdx].a_idx].pos.x, vertices[triangles[tIdx].a_idx].pos.y);
    glm::fvec2 b (vertices[triangles[tIdx].b_idx].pos.x, vertices[triangles[tIdx].b_idx].pos.y);
    glm::fvec2 c (vertices[triangles[tIdx].c_idx].pos.x, vertices[triangles[tIdx].c_idx].pos.y);
    
    d1 = sign(point, a, b);
    d2 = sign(point, b, c);
    d3 = sign(point, c, a);
    
    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    
    return !(has_neg && has_pos);
}

size_t aux::surface::findTriangle(glm::fvec2 coord) {
    for (auto &tri: triangles) {
        auto tIdx = &tri - &triangles[0];
        if (isPointOnTriangle(coord, tIdx)) {
            return tIdx;
        }
    }
    return 0;
}

void aux::surface::setHeight(glm::fvec2 coord, float height) {
    size_t tIdx = findTriangle(coord);
    vertices[triangles[tIdx].a_idx].pos.z += height;
    vertices[triangles[tIdx].b_idx].pos.z += height;
    vertices[triangles[tIdx].c_idx].pos.z += height;
}

void aux::surface::getAreaIndexes(glm::fvec2 center, float radius, std::vector<size_t> &indexes) {
    size_t centerIdx = findTriangle(center);
    for (auto& vert: vertices) {
        glm::fvec2 point(vert.pos.x, vert.pos.y);
        float distance = glm::abs(glm::length(point - center));
        if (distance < radius) {
            auto vIdx = &vert - &vertices[0];
            indexes.push_back(vIdx);
        }
    }
}

bool aux::surface::isIntersected(aux::ray ray) {
    
    for (auto &triangle: triangles) {
        glm::fvec3 intersection;
        auto t = &triangle - &triangles[0];
        if (intersectRayTriangle(ray, t, intersection)) {
            return true;
        }
    }
    return false;
}
