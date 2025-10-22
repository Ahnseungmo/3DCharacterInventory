#pragma once

class MatrixBuffer : public ConstBuffer
{
public:
    MatrixBuffer() : ConstBuffer(&matrix, sizeof(Matrix))
    {
        matrix = XMMatrixIdentity();
    }

    void Set(Matrix value)
    {
        matrix = XMMatrixTranspose(value);
    }

private:
    Matrix matrix;
};

class ViewBuffer : public ConstBuffer
{
private:
    struct Data
    {
        Matrix view;
        Matrix invView;
    };

public:
    ViewBuffer() : ConstBuffer(&data, sizeof(Data))
    {
        data.view = XMMatrixIdentity();
        data.invView = XMMatrixIdentity();
    }

    void Set(Matrix view, Matrix invView)
    {
        data.view = XMMatrixTranspose(view);
        data.invView = XMMatrixTranspose(invView);
    }

private:
    Data data;
};

class ColorBuffer : public ConstBuffer
{
public:
    ColorBuffer() : ConstBuffer(&color, sizeof(Float4))
    {
        color = { 1, 1, 1, 1 };
    }

    void Set(Float4 color) { this->color = color; }
    void Set(float r, float g, float b, float a = 1.0f)
    {
        color = { r, g, b, a };
    }

	Float4* GetColor() { return &color; }

private:
    Float4 color;
};



class FloatValueBuffer : public ConstBuffer
{
public:
    FloatValueBuffer() : ConstBuffer(values, sizeof(float) * 4)
    {
    }

	float* GetValues() { return values; }

private:
    float values[4] = {};
};

class IntValueBuffer : public ConstBuffer
{
public:
    IntValueBuffer() : ConstBuffer(values, sizeof(int) * 4)
    {
    }

    int* GetValues() { return values; }

private:
    int values[4] = {};
};

class LightBuffer : public ConstBuffer
{
public:
    struct Light
    {
        Float4 color = { 1, 1, 1, 1 };
        Float3 direction = { 0, -1, 0 };
        int type = 0;

        Float3 position = { 0, 0, 0 };
        float range = 100.0f;

        float inner = 55.0f;
		float outer = 70.0f;
		float length = 10.0f;
        int active = 1;
    };

    struct Data
    {
        Light lights[MAX_LIGHT];

		UINT lightCount = 1;
        Float3 ambientLight = { 0.1f, 0.1f, 0.1f};
        Float3 ambientCeil = { 0.1f, 0.1f, 0.1f};
        float padding;
    };

public:
    LightBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    }

	Data* GetData() { return &data; }

private:
    Data data;
};

class MaterialBuffer : public ConstBuffer
{
public:
    struct Data
    {
		Float4 diffuse = { 1, 1, 1, 1 };
		Float4 specular = { 1, 1, 1, 1 };
		Float4 ambient = { 1, 1, 1, 1 };
		Float4 emissive = { 0, 0, 0, 1 };

		float shininess = 16;
		int hasNormalMap = 0;
		float padding[2];
    };

public:
    MaterialBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    }

    Data* GetData() { return &data; }

private:
    Data data;
};