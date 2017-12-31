#pragma once

namespace JaegerNet
{
    class NoCopy
    {
    protected:
        NoCopy() noexcept {}
        ~NoCopy() noexcept {}

    private:
        NoCopy(const NoCopy&) = delete;
        const NoCopy& operator=(const NoCopy&) = delete;
    };
}
