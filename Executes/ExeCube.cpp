#include "stdafx.h"
#include "ExeCube.h"

ExeCube::ExeCube(ExecuteValues * values)
	:Execute(values)
	, vertexCount(24), indexCount(36)
	, number(0)
	, number2(0)
	, location(0, 0,0), rotation(0, 0, 0), scale(1, 1)
{
	shader = new Shader(Shaders + L"HA001_CubeTexture.hlsl");
	worldBuffer = new WorldBuffer();

	colorBuffer = new ColorBuffer();

	uvBuffer = new UvBuffer();

	vertices = new VertexTexture[vertexCount];

	//정면
	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	vertices[1].Position = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	vertices[2].Position = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	vertices[3].Position = D3DXVECTOR3(0.5f, 0.5f, -0.5f);

	//우측
	vertices[4].Position = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	vertices[5].Position = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	vertices[6].Position = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	vertices[7].Position = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

	//후면
	vertices[8].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	vertices[9].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	vertices[10].Position = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	vertices[11].Position = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

	//좌측

	vertices[12].Position = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	vertices[13].Position = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	vertices[14].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	vertices[15].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);

	//윗면
	vertices[16].Position = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	vertices[17].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	vertices[18].Position = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	vertices[19].Position = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

	//아랫면
	vertices[20].Position = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	vertices[21].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	vertices[22].Position = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	vertices[23].Position = D3DXVECTOR3(0.5f, -0.5f, 0.5f);


	//정
	vertices[0].Uv = D3DXVECTOR2(0, 1);
	vertices[1].Uv = D3DXVECTOR2(0, 0);
	vertices[2].Uv = D3DXVECTOR2(1, 1);
	vertices[3].Uv = D3DXVECTOR2(1, 0);
	//우
	vertices[4].Uv = D3DXVECTOR2(0, 1);
	vertices[5].Uv = D3DXVECTOR2(0, 0);
	vertices[6].Uv = D3DXVECTOR2(1, 1);
	vertices[7].Uv = D3DXVECTOR2(1, 0);
	//후
	vertices[8].Uv = D3DXVECTOR2(0, 1);
	vertices[9].Uv = D3DXVECTOR2(0, 0);
	vertices[10].Uv = D3DXVECTOR2(1, 1);
	vertices[11].Uv = D3DXVECTOR2(1, 0);
	//좌측
	vertices[12].Uv = D3DXVECTOR2(0, 1);
	vertices[13].Uv = D3DXVECTOR2(0, 0);
	vertices[14].Uv = D3DXVECTOR2(1, 1);
	vertices[15].Uv = D3DXVECTOR2(1, 0);
	//윗면	  
	vertices[16].Uv = D3DXVECTOR2(0, 1);
	vertices[17].Uv = D3DXVECTOR2(0, 0);
	vertices[18].Uv = D3DXVECTOR2(1, 1);
	vertices[19].Uv = D3DXVECTOR2(1, 0);
	//아랫면	 
	vertices[20].Uv = D3DXVECTOR2(0, 1);
	vertices[21].Uv = D3DXVECTOR2(0, 0);
	vertices[22].Uv = D3DXVECTOR2(1, 1);
	vertices[23].Uv = D3DXVECTOR2(1, 0);

	indices = new UINT[indexCount]{ 0,1,2,2,1,3,4,5,6,6,5,7,8,9,10,10,9,11,12,13,14,14,13,15,16,17,18,18,17,19,20,21,22,22,21,23 };
	
	//Set AddressMode
	{
		D3D11_SAMPLER_DESC desc;
		States::GetSamplerDesc(&desc);

		//복사
		States::CreateSampler(&desc, &state[0]);

		////찍어 밀기
		//desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		//desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		//States::CreateSampler(&desc, &state[1]);

		////반사
		//desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		//desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		//States::CreateSampler(&desc, &state[2]);

		////남은 영역 정해진 색으로 채우기
		//desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		//desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		//D3DXCOLOR color = D3DXCOLOR(1, 0, 0, 1);
		//memcpy(&desc.BorderColor, &color, sizeof(D3DXCOLOR));
		//States::CreateSampler(&desc, &state[3]);

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		States::CreateSampler(&desc, &state[1]);

	}

	//CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTexture) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;	//데이터 주소

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}
	//CreateIndexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * indexCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = indices;	//데이터 주소

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(hr));
	}
	//Load SRV
	{
		for (int i = 0; i < 12; i++)
			srv[i] = NULL;

		loadSrv = bind(&ExeCube::SetSRV, this, placeholders::_1);
		//loadSrv = SetSRV;
	}
	//Set CullMode
	{
		D3D11_RASTERIZER_DESC desc;
		States::GetRasterizerDesc(&desc);

		desc.CullMode = D3D11_CULL_NONE;
		States::CreateRasterizer(&desc, &rasterizer[0]);

		desc.FillMode = D3D11_FILL_WIREFRAME;
		States::CreateRasterizer(&desc, &rasterizer[1]);
	}

	for (int i = 0; i < 6; i++) 
		uvoffset[i] = 0.5f;
	

}

ExeCube::~ExeCube()
{
	
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);

	SAFE_DELETE_ARRAY(indices);
	SAFE_DELETE_ARRAY(vertices);

	SAFE_DELETE(uvBuffer);
	SAFE_DELETE(colorBuffer);

	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(shader);


}

void ExeCube::Update()
{

	//D3DXMatrixIdentity(&world);

	D3DXMATRIX X,Y,Z;
	D3DXMatrixIdentity(&X);
	D3DXMatrixIdentity(&Y);
	D3DXMatrixIdentity(&Z);

	D3DXMATRIX T;
	D3DXMatrixIdentity(&T);

	if (Keyboard::Get()->Press(VK_SHIFT)) {
		if (Keyboard::Get()->Press('A'))
			rotation.y += 40.0f*Time::Delta();
		else if (Keyboard::Get()->Press('D'))
			rotation.y -= 40.0f*Time::Delta();

		if (Keyboard::Get()->Press('W'))
			rotation.x -= 40.0f*Time::Delta();
		else if (Keyboard::Get()->Press('S'))
			rotation.x += 40.0f*Time::Delta();

		if (Keyboard::Get()->Press('Q'))
			rotation.z += 40.0f * Time::Delta();
		else if (Keyboard::Get()->Press('E'))
			rotation.z -= 40.0f * Time::Delta();
	}
	else {
		if (Keyboard::Get()->Press('A'))
			location.x -= 3*Time::Delta();
		else if (Keyboard::Get()->Press('D'))
			location.x += 3*Time::Delta();

		if (Keyboard::Get()->Press('W'))
			location.y += 3*Time::Delta();
		else if (Keyboard::Get()->Press('S'))
			location.y -= 3*Time::Delta();
	}

	D3DXMatrixTranslation(&T, location.x, location.y, location.z);

	D3DXMatrixRotationX(&X, Math::ToRadian(rotation.x));
	D3DXMatrixRotationY(&Y, Math::ToRadian(rotation.y));
	D3DXMatrixRotationZ(&Z, Math::ToRadian(rotation.z));


	world = X*Y*Z*T;
}

void ExeCube::PreRender()
{

}

void ExeCube::Render()
{
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//방식
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	
	colorBuffer->SetPSBuffer(0);	//register num
	
	worldBuffer->SetVSBuffer(1);
	
	shader->Render();
	
	D3D::GetDC()->PSSetSamplers(0, 1, &state[number]);
	D3D::GetDC()->RSSetState(rasterizer[number2]);

	worldBuffer->SetMatrix(world);
	worldBuffer->SetVSBuffer(1);

	for (int i = 0; i < 6; i++) {
		D3D::GetDC()->PSSetShaderResources(0, 1, &srv[i * 2]);
		D3D::GetDC()->PSSetShaderResources(1, 1, &srv[i * 2 + 1]);

		uvBuffer->Data.uv_offset = uvoffset[i];
		uvBuffer->SetPSBuffer(1);

		//실제 그려지는 코드
		D3D::GetDC()->DrawIndexed(6, i * 6, 0);
	}

	//D3DXMATRIX world, S, R, T;
	////D3DXMatrixIdentity(&world);	//translation사용시 자동

	//D3DXMatrixScaling(&S, 2, 3, 1);	//x,y,z 스케일값 변경
	//D3DXMatrixRotationY(&R, Math::ToRadian(45));
	////D3DXMatrixTranslation(&T, 2, 0, 0);	//x,y,z 이동



	//world =R;	//SRT순이 일반적 

}

void ExeCube::PostRender()
{
	string str[] = { "front","right","back","left","top","bottom" };

	ImGui::Begin("SetCube");
	{
		ImGui::SliderInt("AddressMode", &number, 0, 1);
		ImGui::SliderInt("CullMode", &number2, 0, 1);
		for (int i = 0; i < 6; i++) {
			if (ImGui::Button((str[i] + " Side Texture1").c_str())) {
				textureList = i * 2;
				Path::OpenFileDialog(L"Box.png", Path::ImageFilter, Textures, loadSrv, NULL);
			}
			if (ImGui::Button((str[i] + " Side Texture2").c_str())) {
				textureList = i * 2 + 1;
				Path::OpenFileDialog(L"Box.png", Path::ImageFilter, Textures, loadSrv, NULL);
			}
			
			ImGui::SliderFloat((str[i] + " Side").c_str(), &uvoffset[i], 0, 1);
		}
	}
	ImGui::End();


}

void ExeCube::ResizeScreen()
{
}

void ExeCube::SetSRV(wstring str)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), str.c_str(), NULL, NULL, &srv[textureList], NULL);
	assert(SUCCEEDED(hr));
}
