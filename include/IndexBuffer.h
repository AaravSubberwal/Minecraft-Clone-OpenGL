class IndexBuffer
{
public:
    IndexBuffer(const unsigned int *data, unsigned int count);
    ~IndexBuffer();
    void bind() const;
    void unbind() const;
    unsigned int getCount() const { return count; }

private:
    unsigned int ID;
    unsigned int count;
};
