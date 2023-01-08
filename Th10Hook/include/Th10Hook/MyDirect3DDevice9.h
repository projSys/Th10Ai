#pragma once

#include "Th10Hook/Common.h"

#include <d3d9.h>

namespace th
{
	class MyDirect3D9;

	class MyDirect3DDevice9 :
		public IDirect3DDevice9
	{
	public:
		MyDirect3DDevice9(IDirect3DDevice9* device, MyDirect3D9* d3d9, HWND window);
		~MyDirect3DDevice9();

		/*** IUnknown methods ***/
		STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) override;
		STDMETHOD_(ULONG, AddRef)(THIS) override;
		STDMETHOD_(ULONG, Release)(THIS) override;

		/*** IDirect3DDevice9 methods ***/
		STDMETHOD(TestCooperativeLevel)(THIS) override;
		STDMETHOD_(UINT, GetAvailableTextureMem)(THIS) override;
		STDMETHOD(EvictManagedResources)(THIS) override;
		STDMETHOD(GetDirect3D)(THIS_ IDirect3D9** ppD3D9) override;
		STDMETHOD(GetDeviceCaps)(THIS_ D3DCAPS9* pCaps) override;
		STDMETHOD(GetDisplayMode)(THIS_ UINT iSwapChain, D3DDISPLAYMODE* pMode) override;
		STDMETHOD(GetCreationParameters)(THIS_ D3DDEVICE_CREATION_PARAMETERS* pParameters) override;
		STDMETHOD(SetCursorProperties)(THIS_ UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap) override;
		STDMETHOD_(void, SetCursorPosition)(THIS_ int X, int Y, DWORD Flags) override;
		STDMETHOD_(BOOL, ShowCursor)(THIS_ BOOL bShow) override;
		STDMETHOD(CreateAdditionalSwapChain)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain) override;
		STDMETHOD(GetSwapChain)(THIS_ UINT iSwapChain, IDirect3DSwapChain9** pSwapChain) override;
		STDMETHOD_(UINT, GetNumberOfSwapChains)(THIS) override;
		STDMETHOD(Reset)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters) override;
		STDMETHOD(Present)(THIS_ CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) override;
		STDMETHOD(GetBackBuffer)(THIS_ UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer) override;
		STDMETHOD(GetRasterStatus)(THIS_ UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus) override;
		STDMETHOD(SetDialogBoxMode)(THIS_ BOOL bEnableDialogs) override;
		STDMETHOD_(void, SetGammaRamp)(THIS_ UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp) override;
		STDMETHOD_(void, GetGammaRamp)(THIS_ UINT iSwapChain, D3DGAMMARAMP* pRamp) override;
		STDMETHOD(CreateTexture)(THIS_ UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle) override;
		STDMETHOD(CreateVolumeTexture)(THIS_ UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle) override;
		STDMETHOD(CreateCubeTexture)(THIS_ UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle) override;
		STDMETHOD(CreateVertexBuffer)(THIS_ UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle) override;
		STDMETHOD(CreateIndexBuffer)(THIS_ UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle) override;
		STDMETHOD(CreateRenderTarget)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) override;
		STDMETHOD(CreateDepthStencilSurface)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) override;
		STDMETHOD(UpdateSurface)(THIS_ IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint) override;
		STDMETHOD(UpdateTexture)(THIS_ IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture) override;
		STDMETHOD(GetRenderTargetData)(THIS_ IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface) override;
		STDMETHOD(GetFrontBufferData)(THIS_ UINT iSwapChain, IDirect3DSurface9* pDestSurface) override;
		STDMETHOD(StretchRect)(THIS_ IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter) override;
		STDMETHOD(ColorFill)(THIS_ IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color) override;
		STDMETHOD(CreateOffscreenPlainSurface)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) override;
		STDMETHOD(SetRenderTarget)(THIS_ DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget) override;
		STDMETHOD(GetRenderTarget)(THIS_ DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget) override;
		STDMETHOD(SetDepthStencilSurface)(THIS_ IDirect3DSurface9* pNewZStencil) override;
		STDMETHOD(GetDepthStencilSurface)(THIS_ IDirect3DSurface9** ppZStencilSurface) override;
		STDMETHOD(BeginScene)(THIS) override;
		STDMETHOD(EndScene)(THIS) override;
		STDMETHOD(Clear)(THIS_ DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) override;
		STDMETHOD(SetTransform)(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) override;
		STDMETHOD(GetTransform)(THIS_ D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix) override;
		STDMETHOD(MultiplyTransform)(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) override;
		STDMETHOD(SetViewport)(THIS_ CONST D3DVIEWPORT9* pViewport) override;
		STDMETHOD(GetViewport)(THIS_ D3DVIEWPORT9* pViewport) override;
		STDMETHOD(SetMaterial)(THIS_ CONST D3DMATERIAL9* pMaterial) override;
		STDMETHOD(GetMaterial)(THIS_ D3DMATERIAL9* pMaterial) override;
		STDMETHOD(SetLight)(THIS_ DWORD Index, CONST D3DLIGHT9* pLight) override;
		STDMETHOD(GetLight)(THIS_ DWORD Index, D3DLIGHT9* pLight) override;
		STDMETHOD(LightEnable)(THIS_ DWORD Index, BOOL Enable) override;
		STDMETHOD(GetLightEnable)(THIS_ DWORD Index, BOOL* pEnable) override;
		STDMETHOD(SetClipPlane)(THIS_ DWORD Index, CONST float* pPlane) override;
		STDMETHOD(GetClipPlane)(THIS_ DWORD Index, float* pPlane) override;
		STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE State, DWORD Value) override;
		STDMETHOD(GetRenderState)(THIS_ D3DRENDERSTATETYPE State, DWORD* pValue) override;
		STDMETHOD(CreateStateBlock)(THIS_ D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB) override;
		STDMETHOD(BeginStateBlock)(THIS) override;
		STDMETHOD(EndStateBlock)(THIS_ IDirect3DStateBlock9** ppSB) override;
		STDMETHOD(SetClipStatus)(THIS_ CONST D3DCLIPSTATUS9* pClipStatus) override;
		STDMETHOD(GetClipStatus)(THIS_ D3DCLIPSTATUS9* pClipStatus) override;
		STDMETHOD(GetTexture)(THIS_ DWORD Stage, IDirect3DBaseTexture9** ppTexture) override;
		STDMETHOD(SetTexture)(THIS_ DWORD Stage, IDirect3DBaseTexture9* pTexture) override;
		STDMETHOD(GetTextureStageState)(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue) override;
		STDMETHOD(SetTextureStageState)(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) override;
		STDMETHOD(GetSamplerState)(THIS_ DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue) override;
		STDMETHOD(SetSamplerState)(THIS_ DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) override;
		STDMETHOD(ValidateDevice)(THIS_ DWORD* pNumPasses) override;
		STDMETHOD(SetPaletteEntries)(THIS_ UINT PaletteNumber, CONST PALETTEENTRY* pEntries) override;
		STDMETHOD(GetPaletteEntries)(THIS_ UINT PaletteNumber, PALETTEENTRY* pEntries) override;
		STDMETHOD(SetCurrentTexturePalette)(THIS_ UINT PaletteNumber) override;
		STDMETHOD(GetCurrentTexturePalette)(THIS_ UINT* PaletteNumber) override;
		STDMETHOD(SetScissorRect)(THIS_ CONST RECT* pRect) override;
		STDMETHOD(GetScissorRect)(THIS_ RECT* pRect) override;
		STDMETHOD(SetSoftwareVertexProcessing)(THIS_ BOOL bSoftware) override;
		STDMETHOD_(BOOL, GetSoftwareVertexProcessing)(THIS) override;
		STDMETHOD(SetNPatchMode)(THIS_ float nSegments) override;
		STDMETHOD_(float, GetNPatchMode)(THIS) override;
		STDMETHOD(DrawPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) override;
		STDMETHOD(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) override;
		STDMETHOD(DrawPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) override;
		STDMETHOD(DrawIndexedPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) override;
		STDMETHOD(ProcessVertices)(THIS_ UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags) override;
		STDMETHOD(CreateVertexDeclaration)(THIS_ CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl) override;
		STDMETHOD(SetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9* pDecl) override;
		STDMETHOD(GetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9** ppDecl) override;
		STDMETHOD(SetFVF)(THIS_ DWORD FVF) override;
		STDMETHOD(GetFVF)(THIS_ DWORD* pFVF) override;
		STDMETHOD(CreateVertexShader)(THIS_ CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader) override;
		STDMETHOD(SetVertexShader)(THIS_ IDirect3DVertexShader9* pShader) override;
		STDMETHOD(GetVertexShader)(THIS_ IDirect3DVertexShader9** ppShader) override;
		STDMETHOD(SetVertexShaderConstantF)(THIS_ UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount) override;
		STDMETHOD(GetVertexShaderConstantF)(THIS_ UINT StartRegister, float* pConstantData, UINT Vector4fCount) override;
		STDMETHOD(SetVertexShaderConstantI)(THIS_ UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount) override;
		STDMETHOD(GetVertexShaderConstantI)(THIS_ UINT StartRegister, int* pConstantData, UINT Vector4iCount) override;
		STDMETHOD(SetVertexShaderConstantB)(THIS_ UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount) override;
		STDMETHOD(GetVertexShaderConstantB)(THIS_ UINT StartRegister, BOOL* pConstantData, UINT BoolCount) override;
		STDMETHOD(SetStreamSource)(THIS_ UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride) override;
		STDMETHOD(GetStreamSource)(THIS_ UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride) override;
		STDMETHOD(SetStreamSourceFreq)(THIS_ UINT StreamNumber, UINT Setting) override;
		STDMETHOD(GetStreamSourceFreq)(THIS_ UINT StreamNumber, UINT* pSetting) override;
		STDMETHOD(SetIndices)(THIS_ IDirect3DIndexBuffer9* pIndexData) override;
		STDMETHOD(GetIndices)(THIS_ IDirect3DIndexBuffer9** ppIndexData) override;
		STDMETHOD(CreatePixelShader)(THIS_ CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader) override;
		STDMETHOD(SetPixelShader)(THIS_ IDirect3DPixelShader9* pShader) override;
		STDMETHOD(GetPixelShader)(THIS_ IDirect3DPixelShader9** ppShader) override;
		STDMETHOD(SetPixelShaderConstantF)(THIS_ UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount) override;
		STDMETHOD(GetPixelShaderConstantF)(THIS_ UINT StartRegister, float* pConstantData, UINT Vector4fCount) override;
		STDMETHOD(SetPixelShaderConstantI)(THIS_ UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount) override;
		STDMETHOD(GetPixelShaderConstantI)(THIS_ UINT StartRegister, int* pConstantData, UINT Vector4iCount) override;
		STDMETHOD(SetPixelShaderConstantB)(THIS_ UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount) override;
		STDMETHOD(GetPixelShaderConstantB)(THIS_ UINT StartRegister, BOOL* pConstantData, UINT BoolCount) override;
		STDMETHOD(DrawRectPatch)(THIS_ UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo) override;
		STDMETHOD(DrawTriPatch)(THIS_ UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo) override;
		STDMETHOD(DeletePatch)(THIS_ UINT Handle) override;
		STDMETHOD(CreateQuery)(THIS_ D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery) override;

	private:
		IDirect3DDevice9* m_device;
		MyDirect3D9* m_d3d9;
	};
}
