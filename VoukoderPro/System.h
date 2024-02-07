#pragma once

#include "../3rdparty/infoware/include/infoware/cpu.hpp"
#include "../3rdparty/infoware/include/infoware/gpu.hpp"
#include "../3rdparty/infoware/include/infoware/system.hpp"
#include "../3rdparty/infoware/include/infoware/version.hpp"
#include "json.hpp"
#include <iostream>
#include <utility>
#include <sstream>

#pragma comment(lib, "version.lib")
#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "wbemuuid.lib")
#if _DEBUG
#pragma comment(lib, "../3rdparty/infoware/lib/infowared.lib")
#else
#pragma comment(lib, "../3rdparty/infoware/lib/infoware.lib")
#endif
#pragma comment(lib, "DXGI.lib")

static const char* cache_type_name(iware::cpu::cache_type_t cache_type) noexcept {
	switch (cache_type) {
	case iware::cpu::cache_type_t::unified:
		return "Unified";
	case iware::cpu::cache_type_t::instruction:
		return "Instruction";
	case iware::cpu::cache_type_t::data:
		return "Data";
	case iware::cpu::cache_type_t::trace:
		return "Trace";
	default:
		return "Unknown";
	}
}

static const char* architecture_name(iware::cpu::architecture_t architecture) noexcept {
	switch (architecture) {
	case iware::cpu::architecture_t::x64:
		return "x64";
	case iware::cpu::architecture_t::arm:
		return "ARM";
	case iware::cpu::architecture_t::itanium:
		return "Itanium";
	case iware::cpu::architecture_t::x86:
		return "x86";
	default:
		return "Unknown";
	}
}

static const char* endianness_name(iware::cpu::endianness_t endianness) noexcept {
	switch (endianness) {
	case iware::cpu::endianness_t::little:
		return "Little-Endian";
	case iware::cpu::endianness_t::big:
		return "Big-Endian";
	default:
		return "Unknown";
	}
}

static const char* kernel_variant_name(iware::system::kernel_t variant) noexcept {
	switch (variant) {
	case iware::system::kernel_t::windows_nt:
		return "Windows NT";
	case iware::system::kernel_t::linux:
		return "Linux";
	case iware::system::kernel_t::darwin:
		return "Darwin";
	default:
		return "Unknown";
	}
}

static const char* vendor_name(iware::gpu::vendor_t vendor) noexcept {
	switch (vendor) {
	case iware::gpu::vendor_t::intel:
		return "Intel";
	case iware::gpu::vendor_t::amd:
		return "AMD";
	case iware::gpu::vendor_t::nvidia:
		return "NVidia";
	case iware::gpu::vendor_t::microsoft:
		return "Microsoft";
	case iware::gpu::vendor_t::qualcomm:
		return "Qualcomm";
	case iware::gpu::vendor_t::apple:
		return "Apple";
	default:
		return "Unknown";
	}
}
