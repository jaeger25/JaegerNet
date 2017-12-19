#pragma once

namespace JaegerNet
{
    class Lobby
    {
    public:
        Lobby(int id);
        virtual ~Lobby();

        int Id() const;

    private:
        int m_id;
    };
}
