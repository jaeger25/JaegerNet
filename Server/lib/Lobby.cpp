#include "Lobby.h"

using namespace JaegerNet;
using namespace std;

Lobby::Lobby(int id) :
    m_id(id)
{

}

Lobby::~Lobby()
{

}

int Lobby::Id() const
{
    return m_id;
}