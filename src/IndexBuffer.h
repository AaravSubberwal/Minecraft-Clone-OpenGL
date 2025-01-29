class IndexBuffer
{
private:
    unsigned int ID;
    unsigned int count;

public:
    IndexBuffer(const unsigned int *data, unsigned int cunt);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;
    inline unsigned int GetCount() { return count; }
};