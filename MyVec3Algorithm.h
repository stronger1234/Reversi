#pragma once

template<typename T>
std::array<T, 3> operator+(const std::array<T, 3> &v1, const std::array<T, 3> &v2) {
	return { v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2] };
}

template<typename T>
std::array<T, 3> operator-(const std::array<T, 3> &v1, const std::array<T, 3> &v2) {
	return { v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2] };
}

template<typename T>
std::array<T, 3> operator*(const std::array<T, 3> &v1, const std::array<T, 3> &v2) {
	return { v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2] };
}

template<typename T>
std::array<T, 3> normalize3v(const std::array<T, 3> &v) {
	T w = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	return { v[0] / w, v[1] / w, v[2] / w };
}

template<typename T>
std::array<T, 3> cross3v(const std::array<T, 3> &v1, const std::array<T, 3> &v2) {
	return {
		v1[1] * v2[2] - v1[2] * v2[1],
		v1[2] * v2[0] - v1[0] * v2[2],
		v1[0] * v2[1] - v1[1] * v2[0]
	};
}

template<typename T>
T dot3v(const std::array<T, 3> &v1, const std::array<T, 3> &v2) {
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

template<typename T>
std::array<T, 3> make3v(const T *p) {
	return { p[0], p[1], p[2] };
}

template<typename T>
bool isRayIntersectPolygon3v(const T *raySrcV, const T *rayAnyV, const T *polyV, unsigned int nPolyV) {
	auto rvs = make3v(raySrcV), rva = make3v(rayAnyV);
	auto dir = normalize3v(rva - rvs);
	auto v0 = make3v(polyV);
	for (auto i = 1U; i < nPolyV - 1; i++) {
		auto v1 = make3v(polyV + 3 * i), v2 = make3v(polyV + 3 * (i + 1));
		auto e1 = v1 - v0;
		auto e2 = v2 - v0;
		auto pvec = cross3v(dir, e2);
		auto det = dot3v(e1, pvec);
		auto tvec = det > 0 ? rvs - v0 : v0 - rvs;
		det = abs(det);
		if (det < 0.0001f) continue;
		auto u = dot3v(tvec, pvec);
		if (u < 0.0f || u > det) continue;
		auto qvec = cross3v(tvec, e1);
		auto v = dot3v(dir, qvec);
		if (v < 0.0f || u + v > det) continue;
		return true;
	}
	return false;
}