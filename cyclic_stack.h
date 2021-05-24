#ifndef CYCLICQUEUE_H
#define CYCLICQUEUE_H

template <typename T, int N>
class CyclicStack
{
public:
    void push(const T &value)
    {
        data[head] = value;

        if (head < N - 1)
            ++head;
        else
            head = 0;

        if (size < N)
            ++size;
    }

    T pop()
    {
        if (size > 0)
        {
            if (head > 0)
                --head;
            else
                head = N - 1;

            --size;

            return data[head];
        }

        return T{};
    }

    void clear()
    {
        size = 0;
        head = 0;
    }

    bool isEmpty()
    {
        return (size == 0);
    }

private:
    T data[N] = {};
    int head = 0;
    int size = 0;
};

#endif // CYCLICQUEUE_H
