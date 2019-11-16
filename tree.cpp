UInt TTrEngine::xdecide_depth0_QP23_v3(Double *dFeatures)
{
if(dFeatures[6] <= 0.66)
{
    if(dFeatures[8] <= 0.03)
    {
        if(dFeatures[0] <= 2.60)
        {
            return 0;
        }
        else
        {
            if(dFeatures[2] <= 0.01)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
    else
    {
        if(dFeatures[3] <= 0.00)
        {
            if(dFeatures[3] <= 0.00)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 0;
        }
    }
}
else
{
    if(dFeatures[2] <= 0.01)
    {
        if(dFeatures[4] <= 21.00)
        {
            if(dFeatures[7] <= 0.16)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if(dFeatures[3] <= 0.04)
        {
            if(dFeatures[1] <= 35.67)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if(dFeatures[5] <= 5.00)
            {
                return 0;
            }
            else
            {
                return 0;
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth0_QP24_v3(Double *dFeatures)
{
if(dFeatures[8] <= 0.03)
{
    if(dFeatures[7] <= 0.41)
    {
        if(dFeatures[1] <= 6.15)
        {
            if(dFeatures[6] <= 0.84)
            {
                if(dFeatures[3] <= 0.00)
                {
                    if(dFeatures[6] <= 0.72)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[3] <= 0.16)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[2] <= 0.03)
                {
                    if(dFeatures[1] <= 1.14)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 0;
                }
            }
        }
        else
        {
            if(dFeatures[5] <= 17.50)
            {
                if(dFeatures[3] <= 1.37)
                {
                    if(dFeatures[1] <= 2351.32)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 183.42)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 192.40)
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
    }
    else
    {
        if(dFeatures[6] <= 0.28)
        {
            if(dFeatures[1] <= 3024.48)
            {
                if(dFeatures[7] <= 0.97)
                {
                    if(dFeatures[0] <= 61.55)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 0.41)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if(dFeatures[3] <= 0.02)
            {
                if(dFeatures[4] <= 10.50)
                {
                    if(dFeatures[6] <= 0.41)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[5] <= 17.50)
                {
                    return 0;
                }
                else
                {
                    return 0;
                }
            }
        }
    }
}
else
{
    if(dFeatures[4] <= 26.50)
    {
        if(dFeatures[7] <= 0.91)
        {
            if(dFeatures[6] <= 0.53)
            {
                if(dFeatures[3] <= 0.05)
                {
                    if(dFeatures[3] <= 0.05)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 467.34)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[9] <= 0.03)
                {
                    if(dFeatures[3] <= 1.22)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
        else
        {
            if(dFeatures[0] <= 0.48)
            {
                if(dFeatures[5] <= 8.00)
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                return 1;
            }
        }
    }
    else
    {
        if(dFeatures[2] <= 0.00)
        {
            if(dFeatures[0] <= 4.60)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if(dFeatures[4] <= 38.50)
            {
                if(dFeatures[7] <= 0.84)
                {
                    if(dFeatures[0] <= 0.44)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 1.52)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[5] <= 40.50)
                {
                    return 0;
                }
                else
                {
                    return 0;
                    }
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth0_QP25_v3(Double *dFeatures)
{
if(dFeatures[8] <= 0.03)
{
    if(dFeatures[7] <= 0.34)
    {
        if(dFeatures[5] <= 49.50)
        {
            if(dFeatures[3] <= 0.02)
            {
                if(dFeatures[7] <= 0.28)
                {
                    if(dFeatures[1] <= 204.42)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 1.23)
                {
                    if(dFeatures[0] <= 0.13)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[6] <= 0.72)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if(dFeatures[7] <= 0.59)
        {
            if(dFeatures[0] <= 1.63)
            {
                if(dFeatures[2] <= 0.24)
                {
                    if(dFeatures[1] <= 0.48)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[2] <= 0.01)
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[6] <= 0.34)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[1] <= 277.81)
            {
                if(dFeatures[7] <= 0.91)
                {
                    if(dFeatures[6] <= 0.28)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 0.55)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[3] <= 0.06)
                {
                    return 1;
                }
                else
                {
                    if(dFeatures[3] <= 0.64)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
    }
}
else
{
    if(dFeatures[4] <= 18.50)
    {
        if(dFeatures[6] <= 0.72)
        {
            if(dFeatures[3] <= 0.01)
            {
                if(dFeatures[1] <= 6.98)
                {
                    if(dFeatures[4] <= 15.00)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if(dFeatures[0] <= 10.67)
                {
                    return 0;
                }
                else
                {
                    return 0;
                }
            }
        }
        else
        {
            if(dFeatures[2] <= 0.01)
            {
                if(dFeatures[1] <= 7.27)
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if(dFeatures[3] <= 0.06)
                {
                    if(dFeatures[0] <= 1.60)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
    }
    else
    {
        if(dFeatures[5] <= 42.50)
        {
            if(dFeatures[3] <= 0.02)
            {
                if(dFeatures[1] <= 0.38)
                {
                    if(dFeatures[5] <= 19.50)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[7] <= 0.22)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[4] <= 133.00)
                {
                    if(dFeatures[7] <= 0.09)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
        else
        {
            if(dFeatures[7] <= 0.84)
            {
                if(dFeatures[4] <= 79.50)
                {
                    return 0;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[4] <= 82.00)
                {
                    return 1;
                }
                else
                {
                    return 0;
                    }
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth0_QP28_v3(Double *dFeatures)
{
if(dFeatures[7] <= 0.09)
{
    if(dFeatures[3] <= 0.03)
    {
        if(dFeatures[8] <= 0.03)
        {
            if(dFeatures[1] <= 0.10)
            {
                if(dFeatures[0] <= 15.57)
                {
                    if(dFeatures[0] <= 0.32)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 27.73)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 21.88)
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[9] <= 0.44)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[1] <= 124.28)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
    }
    else
    {
        if(dFeatures[0] <= 53.84)
        {
            return 0;
        }
        else
        {
            if(dFeatures[0] <= 126.26)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
}
else
{
    if(dFeatures[2] <= 0.00)
    {
        if(dFeatures[0] <= 42.00)
        {
            if(dFeatures[1] <= 8.10)
            {
                if(dFeatures[1] <= 1.17)
                {
                    if(dFeatures[4] <= 31.00)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[9] <= 0.09)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
        }
        else
        {
            if(dFeatures[1] <= 321.45)
            {
                return 1;
            }
            else
            {
                if(dFeatures[0] <= 322.62)
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
        }
    }
    else
    {
        if(dFeatures[4] <= 1.00)
        {
            if(dFeatures[7] <= 0.16)
            {
                if(dFeatures[3] <= 0.05)
                {
                    if(dFeatures[3] <= 0.01)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 0.92)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 0.64)
                {
                    if(dFeatures[1] <= 1.72)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[3] <= 0.01)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[9] <= 0.22)
            {
                if(dFeatures[0] <= 190.06)
                {
                    return 0;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[2] <= 0.56)
                {
                    if(dFeatures[1] <= 203.97)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 0;
                    }
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth0_QP29_v3(Double *dFeatures)
{
if(dFeatures[8] <= 0.03)
{
    if(dFeatures[7] <= 0.28)
    {
        if(dFeatures[7] <= 0.16)
        {
            if(dFeatures[1] <= 350.41)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if(dFeatures[5] <= 17.00)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
    else
    {
        if(dFeatures[4] <= 6.50)
        {
            if(dFeatures[3] <= 0.04)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if(dFeatures[1] <= 0.49)
            {
                return 0;
            }
            else
            {
                return 0;
            }
        }
    }
}
else
{
    if(dFeatures[1] <= 0.64)
    {
        if(dFeatures[8] <= 0.09)
        {
            if(dFeatures[4] <= 29.50)
            {
                return 0;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if(dFeatures[6] <= 0.16)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
    else
    {
        if(dFeatures[4] <= 24.50)
        {
            return 0;
        }
        else
        {
            return 0;
            }
        }
    }
}

UInt TTrEngine::xdecide_depth0_QP30_v3(Double *dFeatures)
{
if(dFeatures[8] <= 0.03)
{
    if(dFeatures[7] <= 0.16)
    {
        if(dFeatures[7] <= 0.09)
        {
            if(dFeatures[2] <= 1.67)
            {
                if(dFeatures[3] <= 0.80)
                {
                    if(dFeatures[9] <= 0.47)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.25)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if(dFeatures[5] <= 1.50)
            {
                if(dFeatures[2] <= 0.35)
                {
                    if(dFeatures[1] <= 1001.66)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[2] <= 0.00)
                {
                    if(dFeatures[5] <= 32.50)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[5] <= 19.00)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[5] <= 3.50)
        {
            if(dFeatures[7] <= 0.28)
            {
                if(dFeatures[0] <= 605.36)
                {
                    if(dFeatures[2] <= 0.63)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[0] <= 2.87)
                {
                    if(dFeatures[1] <= 1.47)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.78)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[0] <= 0.21)
            {
                if(dFeatures[4] <= 7.50)
                {
                    if(dFeatures[6] <= 0.59)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[7] <= 0.81)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[2] <= 0.00)
                {
                    if(dFeatures[0] <= 33.89)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 3.24)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
    }
}
else
{
    if(dFeatures[0] <= 0.45)
    {
        if(dFeatures[4] <= 29.50)
        {
            if(dFeatures[3] <= 2.38)
            {
                if(dFeatures[6] <= 0.03)
                {
                    if(dFeatures[4] <= 8.00)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[7] <= 0.66)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if(dFeatures[1] <= 0.33)
            {
                return 0;
            }
            else
            {
                if(dFeatures[1] <= 0.45)
                {
                    if(dFeatures[7] <= 0.47)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[5] <= 34.50)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[4] <= 22.50)
        {
            if(dFeatures[0] <= 1.55)
            {
                if(dFeatures[0] <= 1.48)
                {
                    if(dFeatures[5] <= 14.50)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if(dFeatures[2] <= 0.47)
                {
                    if(dFeatures[2] <= 0.44)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 5.33)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[0] <= 0.72)
            {
                if(dFeatures[1] <= 1.16)
                {
                    if(dFeatures[4] <= 92.50)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[7] <= 0.50)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[2] <= 4.02)
                {
                    if(dFeatures[5] <= 14.50)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[3] <= 4.55)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                        }
                    }
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth0_QP33_v3(Double *dFeatures)
{
if(dFeatures[7] <= 0.09)
{
    if(dFeatures[2] <= 0.00)
    {
        if(dFeatures[9] <= 0.03)
        {
            if(dFeatures[6] <= 0.97)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if(dFeatures[8] <= 0.03)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
    else
    {
        if(dFeatures[0] <= 1.82)
        {
            if(dFeatures[2] <= 0.00)
            {
                return 0;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if(dFeatures[6] <= 0.91)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
    }
}
else
{
    if(dFeatures[3] <= 0.00)
    {
        if(dFeatures[2] <= 0.00)
        {
            if(dFeatures[1] <= 161.29)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if(dFeatures[6] <= 0.72)
        {
            return 0;
        }
        else
        {
            return 0;
            }
        }
    }
}

UInt TTrEngine::xdecide_depth0_QP34_v3(Double *dFeatures)
{
if(dFeatures[6] <= 0.66)
{
    if(dFeatures[1] <= 2.30)
    {
        if(dFeatures[5] <= 5.50)
        {
            if(dFeatures[5] <= 2.50)
            {
                if(dFeatures[1] <= 1.20)
                {
                    if(dFeatures[0] <= 0.60)
                    {
                        if(dFeatures[2] <= 0.03)
                        {
                            if(dFeatures[2] <= 0.03)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[3] <= 1.25)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[3] <= 0.04)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.17)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if(dFeatures[4] <= 4.50)
                {
                    if(dFeatures[0] <= 1.24)
                    {
                        if(dFeatures[2] <= 0.03)
                        {
                            if(dFeatures[6] <= 0.47)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 1.22)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[3] <= 0.24)
                    {
                        if(dFeatures[4] <= 9.50)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[3] <= 0.53)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[1] <= 0.25)
            {
                if(dFeatures[4] <= 33.50)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[2] <= 0.58)
                {
                    if(dFeatures[1] <= 2.15)
                    {
                        if(dFeatures[0] <= 0.91)
                        {
                            if(dFeatures[3] <= 0.06)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[1] <= 0.88)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 2.25)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
    }
    else
    {
        if(dFeatures[8] <= 0.03)
        {
            if(dFeatures[6] <= 0.47)
            {
                if(dFeatures[5] <= 2.50)
                {
                    if(dFeatures[2] <= 0.01)
                    {
                        return 0;
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.07)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[9] <= 0.12)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        return 1;
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.16)
                        {
                            if(dFeatures[2] <= 0.14)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[4] <= 35.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[2] <= 0.00)
                {
                    if(dFeatures[9] <= 0.28)
                    {
                        return 1;
                    }
                    else
                    {
                        if(dFeatures[0] <= 179.50)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    if(dFeatures[4] <= 16.50)
                    {
                        if(dFeatures[3] <= 0.02)
                        {
                            if(dFeatures[6] <= 0.59)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.05)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 49.16)
                        {
                            if(dFeatures[0] <= 30.88)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[6] <= 0.59)
            {
                if(dFeatures[0] <= 1.62)
                {
                    if(dFeatures[0] <= 1.54)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.58)
                    {
                        if(dFeatures[3] <= 0.43)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[3] <= 0.44)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.58)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[3] <= 0.38)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[1] <= 2.76)
                {
                    return 1;
                }
                else
                {
                    if(dFeatures[3] <= 1.57)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
    }
}
else
{
    if(dFeatures[6] <= 0.78)
    {
        if(dFeatures[8] <= 0.03)
        {
            if(dFeatures[0] <= 0.55)
            {
                if(dFeatures[2] <= 0.45)
                {
                    if(dFeatures[4] <= 3.50)
                    {
                        if(dFeatures[0] <= 0.14)
                        {
                            if(dFeatures[0] <= 0.09)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[3] <= 0.00)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[5] <= 16.50)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[5] <= 28.50)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[2] <= 0.00)
                {
                    if(dFeatures[0] <= 12.51)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[4] <= 3.50)
                    {
                        if(dFeatures[2] <= 0.01)
                        {
                            if(dFeatures[6] <= 0.72)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[7] <= 0.22)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[7] <= 0.09)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[2] <= 1.09)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[0] <= 0.42)
            {
                return 1;
            }
            else
            {
                if(dFeatures[1] <= 383.91)
                {
                    if(dFeatures[6] <= 0.72)
                    {
                        if(dFeatures[1] <= 2.69)
                        {
                            if(dFeatures[0] <= 0.77)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[3] <= 0.01)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.77)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
    }
    else
    {
        if(dFeatures[7] <= 0.09)
        {
            if(dFeatures[8] <= 0.03)
            {
                if(dFeatures[3] <= 0.02)
                {
                    return 1;
                }
                else
                {
                    if(dFeatures[5] <= 12.50)
                    {
                        if(dFeatures[0] <= 0.07)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 14.34)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[3] <= 0.07)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
        }
        else
        {
            if(dFeatures[5] <= 3.50)
            {
                if(dFeatures[2] <= 0.06)
                {
                    if(dFeatures[9] <= 0.03)
                    {
                        if(dFeatures[3] <= 0.03)
                        {
                            if(dFeatures[3] <= 0.03)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 0.16)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 124.78)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    if(dFeatures[1] <= 239.67)
                    {
                        if(dFeatures[2] <= 0.07)
                        {
                            if(dFeatures[0] <= 0.47)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[6] <= 0.84)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 6.76)
                {
                    if(dFeatures[0] <= 0.10)
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.09)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[0] <= 1.16)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[4] <= 11.00)
                    {
                        return 0;
                    }
                    else
                    {
                        if(dFeatures[0] <= 29.92)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth0_QP35_v3(Double *dFeatures)
{
if(dFeatures[8] <= 0.03)
{
    if(dFeatures[7] <= 0.22)
    {
        if(dFeatures[2] <= 0.05)
        {
            if(dFeatures[3] <= 0.00)
            {
                if(dFeatures[0] <= 0.03)
                {
                    return 1;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if(dFeatures[0] <= 0.33)
                {
                    if(dFeatures[2] <= 0.04)
                    {
                        if(dFeatures[2] <= 0.01)
                        {
                            if(dFeatures[2] <= 0.01)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.04)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    if(dFeatures[1] <= 0.30)
                    {
                        if(dFeatures[6] <= 0.84)
                        {
                            if(dFeatures[3] <= 0.01)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[6] <= 0.84)
                        {
                            if(dFeatures[2] <= 0.01)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 0.34)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[0] <= 10.65)
            {
                if(dFeatures[2] <= 0.05)
                {
                    return 0;
                }
                else
                {
                    if(dFeatures[4] <= 31.00)
                    {
                        if(dFeatures[2] <= 2.10)
                        {
                            if(dFeatures[0] <= 0.35)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[3] <= 0.31)
                {
                    if(dFeatures[2] <= 0.08)
                    {
                        if(dFeatures[1] <= 21.48)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[3] <= 0.24)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[7] <= 0.09)
                        {
                            if(dFeatures[2] <= 1.01)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[3] <= 0.27)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[3] <= 0.70)
                    {
                        if(dFeatures[0] <= 14.70)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.15)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[6] <= 0.72)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.60)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[0] <= 1.03)
        {
            if(dFeatures[0] <= 0.48)
            {
                if(dFeatures[4] <= 28.00)
                {
                    if(dFeatures[2] <= 0.01)
                    {
                        if(dFeatures[2] <= 0.01)
                        {
                            if(dFeatures[1] <= 0.56)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.07)
                        {
                            if(dFeatures[3] <= 0.04)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.08)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[1] <= 1.70)
                {
                    if(dFeatures[2] <= 1.12)
                    {
                        if(dFeatures[2] <= 0.30)
                        {
                            if(dFeatures[7] <= 0.41)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[3] <= 2.45)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[3] <= 4.49)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[6] <= 0.41)
                    {
                        return 0;
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.40)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.47)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[6] <= 0.41)
            {
                if(dFeatures[3] <= 1.47)
                {
                    if(dFeatures[2] <= 0.01)
                    {
                        if(dFeatures[0] <= 1.67)
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 1.12)
                        {
                            if(dFeatures[3] <= 1.01)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[2] <= 0.43)
                {
                    if(dFeatures[5] <= 7.50)
                    {
                        if(dFeatures[3] <= 0.12)
                        {
                            if(dFeatures[1] <= 0.70)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[7] <= 0.28)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[3] <= 0.00)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[5] <= 10.50)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[0] <= 6.19)
                    {
                        if(dFeatures[5] <= 3.50)
                        {
                            if(dFeatures[2] <= 0.49)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[4] <= 8.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 268.64)
                        {
                            if(dFeatures[1] <= 53.48)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.83)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
}
else
{
    if(dFeatures[0] <= 1.35)
    {
        if(dFeatures[0] <= 0.57)
        {
            if(dFeatures[2] <= 0.03)
            {
                return 1;
            }
            else
            {
                if(dFeatures[2] <= 0.04)
                {
                    return 0;
                }
                else
                {
                    if(dFeatures[1] <= 0.29)
                    {
                        return 0;
                    }
                    else
                    {
                        if(dFeatures[3] <= 0.09)
                        {
                            if(dFeatures[0] <= 0.35)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.29)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[1] <= 2.06)
            {
                if(dFeatures[3] <= 0.05)
                {
                    if(dFeatures[2] <= 0.01)
                    {
                        return 0;
                    }
                    else
                    {
                        if(dFeatures[6] <= 0.84)
                        {
                            if(dFeatures[8] <= 0.22)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    if(dFeatures[3] <= 0.06)
                    {
                        return 0;
                    }
                    else
                    {
                        if(dFeatures[1] <= 1.59)
                        {
                            if(dFeatures[3] <= 0.16)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[4] <= 15.50)
                {
                    if(dFeatures[1] <= 2.93)
                    {
                        return 1;
                    }
                    else
                    {
                        if(dFeatures[1] <= 2.95)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[0] <= 1.23)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[1] <= 2.47)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[8] <= 0.09)
        {
            if(dFeatures[5] <= 5.50)
            {
                if(dFeatures[0] <= 6.98)
                {
                    if(dFeatures[1] <= 7.97)
                    {
                        if(dFeatures[6] <= 0.47)
                        {
                            if(dFeatures[0] <= 1.72)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[1] <= 1.63)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 4.89)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[0] <= 6.10)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[6] <= 0.72)
                    {
                        if(dFeatures[3] <= 0.30)
                        {
                            if(dFeatures[3] <= 0.18)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[6] <= 0.53)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 363.48)
                        {
                            if(dFeatures[2] <= 0.53)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 2.10)
                {
                    if(dFeatures[0] <= 1.82)
                    {
                        if(dFeatures[4] <= 9.00)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[5] <= 20.50)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[4] <= 12.00)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.02)
                    {
                        return 0;
                    }
                    else
                    {
                        if(dFeatures[3] <= 0.05)
                        {
                            if(dFeatures[1] <= 109.72)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 361.38)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[2] <= 2.54)
            {
                if(dFeatures[4] <= 5.50)
                {
                    if(dFeatures[2] <= 1.79)
                    {
                        if(dFeatures[4] <= 4.50)
                        {
                            if(dFeatures[1] <= 3.98)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[3] <= 0.66)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 15.80)
                    {
                        if(dFeatures[0] <= 14.46)
                        {
                            if(dFeatures[0] <= 12.45)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                return 1;
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth0_QP38_v3(Double *dFeatures)
{
if(dFeatures[7] <= 0.09)
{
    if(dFeatures[3] <= 0.00)
    {
        if(dFeatures[0] <= 308.98)
        {
            if(dFeatures[0] <= 0.03)
            {
                return 1;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if(dFeatures[2] <= 0.26)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
    else
    {
        if(dFeatures[7] <= 0.03)
        {
            if(dFeatures[1] <= 1.02)
            {
                return 1;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if(dFeatures[0] <= 46.43)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
    }
}
else
{
    if(dFeatures[6] <= 0.66)
    {
        if(dFeatures[0] <= 0.46)
        {
            if(dFeatures[0] <= 0.39)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if(dFeatures[9] <= 0.22)
            {
                return 1;
            }
            else
            {
                return 1;
            }
        }
    }
    else
    {
        if(dFeatures[6] <= 0.84)
        {
            if(dFeatures[1] <= 1.75)
            {
                return 1;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if(dFeatures[1] <= 74.10)
            {
                return 0;
            }
            else
            {
                return 1;
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth0_QP39_v3(Double *dFeatures)
{
if(dFeatures[6] <= 0.66)
{
    if(dFeatures[0] <= 2.64)
    {
        if(dFeatures[0] <= 1.02)
        {
            if(dFeatures[5] <= 24.00)
            {
                if(dFeatures[2] <= 0.00)
                {
                    if(dFeatures[7] <= 0.41)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if(dFeatures[2] <= 0.00)
            {
                return 0;
            }
            else
            {
                if(dFeatures[5] <= 2.50)
                {
                    if(dFeatures[0] <= 1.21)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 3.65)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[6] <= 0.53)
        {
            if(dFeatures[3] <= 0.00)
            {
                return 1;
            }
            else
            {
                if(dFeatures[1] <= 45.63)
                {
                    if(dFeatures[9] <= 0.03)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[7] <= 0.22)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[9] <= 0.16)
            {
                if(dFeatures[2] <= 0.25)
                {
                    if(dFeatures[2] <= 0.06)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 4.63)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[1] <= 182.78)
                {
                    return 1;
                }
                else
                {
                    if(dFeatures[3] <= 0.36)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
    }
}
else
{
    if(dFeatures[0] <= 3.07)
    {
        if(dFeatures[7] <= 0.16)
        {
            if(dFeatures[8] <= 0.03)
            {
                if(dFeatures[1] <= 1.06)
                {
                    if(dFeatures[2] <= 0.15)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 0.40)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if(dFeatures[0] <= 0.94)
            {
                if(dFeatures[3] <= 0.55)
                {
                    if(dFeatures[1] <= 1.26)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 0.98)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[3] <= 0.14)
                {
                    if(dFeatures[0] <= 1.18)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 1.75)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[2] <= 0.23)
        {
            if(dFeatures[3] <= 0.00)
            {
                return 1;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if(dFeatures[1] <= 20.59)
            {
                if(dFeatures[3] <= 0.52)
                {
                    if(dFeatures[0] <= 4.35)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.88)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[6] <= 0.91)
                {
                    if(dFeatures[3] <= 1.61)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[3] <= 0.29)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                        }
                    }
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth0_QP40_v3(Double *dFeatures)
{
if(dFeatures[6] <= 0.66)
{
    if(dFeatures[8] <= 0.03)
    {
        if(dFeatures[0] <= 2.27)
        {
            if(dFeatures[1] <= 1.53)
            {
                return 1;
            }
            else
            {
                if(dFeatures[2] <= 0.00)
                {
                    return 0;
                }
                else
                {
                    if(dFeatures[0] <= 0.68)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[7] <= 0.41)
            {
                if(dFeatures[2] <= 1.24)
                {
                    if(dFeatures[1] <= 192.87)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[7] <= 0.22)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[1] <= 436.00)
                {
                    if(dFeatures[0] <= 44.29)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 659.50)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[0] <= 55.28)
        {
            if(dFeatures[7] <= 0.53)
            {
                if(dFeatures[4] <= 0.50)
                {
                    if(dFeatures[1] <= 6.23)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[5] <= 17.50)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[1] <= 4.92)
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 37.42)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[6] <= 0.41)
            {
                if(dFeatures[3] <= 0.01)
                {
                    if(dFeatures[4] <= 7.50)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[1] <= 336.01)
                {
                    return 0;
                }
                else
                {
                    if(dFeatures[0] <= 288.15)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
    }
}
else
{
    if(dFeatures[0] <= 3.80)
    {
        if(dFeatures[0] <= 0.71)
        {
            if(dFeatures[7] <= 0.09)
            {
                return 1;
            }
            else
            {
                if(dFeatures[0] <= 0.19)
                {
                    if(dFeatures[1] <= 0.53)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[5] <= 3.50)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[5] <= 24.00)
            {
                if(dFeatures[2] <= 0.00)
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 0.72)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                return 0;
            }
        }
    }
    else
    {
        if(dFeatures[6] <= 0.78)
        {
            if(dFeatures[3] <= 0.29)
            {
                if(dFeatures[0] <= 3.90)
                {
                    return 0;
                }
                else
                {
                    if(dFeatures[8] <= 0.03)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 11.36)
                {
                    if(dFeatures[1] <= 5.86)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[3] <= 0.43)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[6] <= 0.91)
            {
                if(dFeatures[3] <= 0.02)
                {
                    if(dFeatures[0] <= 8.22)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[3] <= 0.03)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 780.37)
                {
                    if(dFeatures[2] <= 1.19)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 0;
                    }
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth1_QP23_v3(Double *dFeatures)
{
if(dFeatures[4] <= 5.50)
{
    if(dFeatures[6] <= 0.62)
    {
        if(dFeatures[0] <= 6.81)
        {
            if(dFeatures[8] <= 0.12)
            {
                if(dFeatures[0] <= 0.50)
                {
                    if(dFeatures[3] <= 0.49)
                    {
                        if(dFeatures[9] <= 0.12)
                        {
                            if(dFeatures[2] <= 0.49)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.02)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 0.32)
                        {
                            if(dFeatures[7] <= 0.62)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    if(dFeatures[3] <= 0.02)
                    {
                        if(dFeatures[2] <= 0.03)
                        {
                            if(dFeatures[5] <= 3.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[3] <= 0.01)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.02)
                        {
                            if(dFeatures[0] <= 0.68)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.08)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[6] <= 0.38)
                {
                    if(dFeatures[2] <= 0.07)
                    {
                        if(dFeatures[2] <= 0.06)
                        {
                            if(dFeatures[0] <= 0.19)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.32)
                        {
                            if(dFeatures[3] <= 0.22)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[3] <= 0.58)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[1] <= 0.22)
                    {
                        if(dFeatures[0] <= 0.30)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 3.27)
                        {
                            if(dFeatures[0] <= 0.22)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[2] <= 0.82)
            {
                if(dFeatures[2] <= 0.01)
                {
                    if(dFeatures[6] <= 0.38)
                    {
                        if(dFeatures[7] <= 0.38)
                        {
                            if(dFeatures[8] <= 0.62)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 17.01)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[0] <= 17.87)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[8] <= 0.12)
                    {
                        if(dFeatures[2] <= 0.02)
                        {
                            if(dFeatures[0] <= 12.46)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[5] <= 1.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.07)
                        {
                            if(dFeatures[9] <= 0.12)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.43)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[7] <= 0.62)
                {
                    if(dFeatures[3] <= 1.55)
                    {
                        if(dFeatures[0] <= 45.25)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[3] <= 2.53)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[2] <= 1.06)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[3] <= 0.14)
                    {
                        if(dFeatures[3] <= 0.08)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[3] <= 0.00)
        {
            if(dFeatures[6] <= 0.88)
            {
                if(dFeatures[0] <= 1.26)
                {
                    if(dFeatures[1] <= 1.75)
                    {
                        if(dFeatures[4] <= 2.00)
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 0.10)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[2] <= 2.63)
                    {
                        if(dFeatures[2] <= 0.01)
                        {
                            if(dFeatures[0] <= 7.49)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[3] <= 0.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 15.33)
                {
                    if(dFeatures[0] <= 1.03)
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 0.31)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.02)
                    {
                        if(dFeatures[0] <= 17.87)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[1] <= 1.66)
            {
                if(dFeatures[2] <= 0.07)
                {
                    if(dFeatures[2] <= 0.01)
                    {
                        if(dFeatures[1] <= 0.23)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 0.07)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[0] <= 5.88)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[2] <= 0.76)
                {
                    if(dFeatures[3] <= 0.81)
                    {
                        if(dFeatures[1] <= 23.08)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[8] <= 0.12)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    return 0;
                }
            }
        }
    }
}
else
{
    if(dFeatures[4] <= 24.50)
    {
        if(dFeatures[9] <= 0.12)
        {
            if(dFeatures[8] <= 0.12)
            {
                if(dFeatures[0] <= 3.79)
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            if(dFeatures[4] <= 6.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[7] <= 0.62)
                        {
                            if(dFeatures[5] <= 8.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[3] <= 0.02)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[5] <= 21.50)
                    {
                        if(dFeatures[0] <= 5.40)
                        {
                            if(dFeatures[3] <= 0.05)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 12.95)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 3.68)
                {
                    if(dFeatures[4] <= 11.50)
                    {
                        if(dFeatures[1] <= 0.26)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 0.03)
                        {
                            if(dFeatures[3] <= 0.01)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[1] <= 9.46)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.14)
                    {
                        if(dFeatures[1] <= 8.31)
                        {
                            if(dFeatures[4] <= 11.50)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[5] <= 22.50)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[6] <= 0.12)
                        {
                            if(dFeatures[3] <= 0.18)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[1] <= 106.88)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[6] <= 0.12)
            {
                if(dFeatures[3] <= 0.00)
                {
                    if(dFeatures[5] <= 6.50)
                    {
                        if(dFeatures[0] <= 210.22)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[8] <= 0.12)
                    {
                        if(dFeatures[5] <= 18.50)
                        {
                            if(dFeatures[2] <= 0.84)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[2] <= 0.49)
                {
                    if(dFeatures[4] <= 22.50)
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[0] <= 464.32)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 3.34)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    return 0;
                }
            }
        }
    }
    else
    {
        if(dFeatures[4] <= 38.50)
        {
            if(dFeatures[3] <= 4.24)
            {
                if(dFeatures[5] <= 9.50)
                {
                    return 1;
                }
                else
                {
                    if(dFeatures[8] <= 0.38)
                    {
                        if(dFeatures[0] <= 0.14)
                        {
                            if(dFeatures[4] <= 37.50)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 3.25)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[7] <= 0.12)
                        {
                            if(dFeatures[5] <= 13.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[1] <= 341.48)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if(dFeatures[5] <= 140.50)
            {
                if(dFeatures[5] <= 43.50)
                {
                    if(dFeatures[1] <= 11.79)
                    {
                        if(dFeatures[1] <= 9.81)
                        {
                            if(dFeatures[4] <= 81.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[1] <= 10.90)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            if(dFeatures[7] <= 0.12)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[6] <= 0.38)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[7] <= 0.62)
                    {
                        if(dFeatures[3] <= 0.00)
                        {
                            if(dFeatures[0] <= 7.63)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[4] <= 151.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 1.40)
                        {
                            if(dFeatures[3] <= 0.01)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
            else
            {
                return 1;
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth1_QP24_v3(Double *dFeatures)
{
if(dFeatures[4] <= 9.50)
{
    if(dFeatures[3] <= 0.00)
    {
        return 0;
    }
    else
    {
        return 0;
    }
}
else
{
    if(dFeatures[1] <= 0.58)
    {
        return 0;
    }
    else
    {
        return 0;
        }
    }
}

UInt TTrEngine::xdecide_depth1_QP25_v3(Double *dFeatures)
{
if(dFeatures[5] <= 18.50)
{
    if(dFeatures[8] <= 0.12)
    {
        if(dFeatures[7] <= 0.38)
        {
            if(dFeatures[0] <= 0.06)
            {
                if(dFeatures[9] <= 0.38)
                {
                    if(dFeatures[3] <= 0.00)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[4] <= 18.00)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[5] <= 11.50)
                {
                    if(dFeatures[0] <= 1829.14)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[3] <= 0.16)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[4] <= 12.50)
            {
                if(dFeatures[0] <= 0.25)
                {
                    return 0;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[1] <= 0.72)
                {
                    if(dFeatures[5] <= 12.50)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 141.43)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[0] <= 0.58)
        {
            if(dFeatures[3] <= 0.87)
            {
                if(dFeatures[8] <= 0.38)
                {
                    if(dFeatures[0] <= 0.24)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[3] <= 0.02)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[2] <= 1.58)
                {
                    if(dFeatures[4] <= 3.50)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 0.23)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[5] <= 9.50)
            {
                if(dFeatures[3] <= 0.56)
                {
                    if(dFeatures[6] <= 0.38)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 1.51)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 2.63)
                {
                    if(dFeatures[2] <= 0.01)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[3] <= 1.77)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
    }
}
else
{
    if(dFeatures[0] <= 0.45)
    {
        if(dFeatures[5] <= 31.50)
        {
            if(dFeatures[8] <= 0.38)
            {
                if(dFeatures[5] <= 26.50)
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[4] <= 75.50)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 0.12)
                {
                    return 1;
                }
                else
                {
                    if(dFeatures[4] <= 45.50)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[1] <= 0.29)
            {
                if(dFeatures[0] <= 0.24)
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[7] <= 0.38)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 0.44)
                {
                    if(dFeatures[1] <= 0.36)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
    }
    else
    {
        if(dFeatures[4] <= 70.50)
        {
            if(dFeatures[5] <= 23.50)
            {
                if(dFeatures[3] <= 2.29)
                {
                    if(dFeatures[8] <= 0.38)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[2] <= 1.25)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[5] <= 51.50)
                {
                    if(dFeatures[0] <= 1081.52)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    return 0;
                }
            }
        }
        else
        {
            if(dFeatures[1] <= 5.39)
            {
                if(dFeatures[3] <= 0.01)
                {
                    if(dFeatures[5] <= 26.50)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[2] <= 3.05)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[5] <= 58.50)
                {
                    return 0;
                }
                else
                {
                    return 0;
                    }
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth1_QP28_v3(Double *dFeatures)
{
if(dFeatures[5] <= 2.50)
{
    if(dFeatures[3] <= 0.00)
    {
        if(dFeatures[7] <= 0.12)
        {
            if(dFeatures[2] <= 0.05)
            {
                if(dFeatures[0] <= 1.03)
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            if(dFeatures[9] <= 0.62)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 0.51)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 0.91)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 0;
                            }
                            else
                            {
                                if(dFeatures[2] <= 0.01)
                                {
                                    if(dFeatures[2] <= 0.00)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if(dFeatures[0] <= 0.21)
                {
                    return 0;
                }
                else
                {
                    if(dFeatures[2] <= 0.06)
                    {
                        if(dFeatures[0] <= 4.70)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 0.86)
                        {
                            if(dFeatures[0] <= 0.29)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 13.47)
                            {
                                if(dFeatures[2] <= 0.09)
                                {
                                    return 1;
                                }
                                else
                                {
                                    if(dFeatures[0] <= 3.66)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[0] <= 4.26)
            {
                if(dFeatures[2] <= 0.01)
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            if(dFeatures[0] <= 1.99)
                            {
                                if(dFeatures[0] <= 1.07)
                                {
                                    if(dFeatures[0] <= 0.99)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                if(dFeatures[0] <= 2.04)
                                {
                                    return 0;
                                }
                                else
                                {
                                    if(dFeatures[6] <= 0.62)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 0;
                            }
                            else
                            {
                                if(dFeatures[5] <= 1.00)
                                {
                                    if(dFeatures[9] <= 0.38)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[7] <= 0.38)
                        {
                            if(dFeatures[0] <= 0.10)
                            {
                                return 0;
                            }
                            else
                            {
                                if(dFeatures[2] <= 0.00)
                                {
                                    return 0;
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 1.38)
                            {
                                if(dFeatures[2] <= 0.00)
                                {
                                    return 0;
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                            else
                            {
                                if(dFeatures[0] <= 2.29)
                                {
                                    return 0;
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.01)
                    {
                        if(dFeatures[0] <= 2.13)
                        {
                            if(dFeatures[6] <= 0.62)
                            {
                                return 0;
                            }
                            else
                            {
                                if(dFeatures[0] <= 0.65)
                                {
                                    return 1;
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 4.07)
                        {
                            if(dFeatures[0] <= 1.71)
                            {
                                if(dFeatures[0] <= 0.90)
                                {
                                    if(dFeatures[2] <= 0.01)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[2] <= 0.01)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[2] <= 2.54)
                                {
                                    if(dFeatures[9] <= 0.12)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[2] <= 0.42)
                {
                    if(dFeatures[2] <= 0.06)
                    {
                        if(dFeatures[1] <= 36.93)
                        {
                            if(dFeatures[2] <= 0.01)
                            {
                                if(dFeatures[2] <= 0.01)
                                {
                                    if(dFeatures[2] <= 0.00)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 85.77)
                            {
                                if(dFeatures[0] <= 39.58)
                                {
                                    return 1;
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[9] <= 0.12)
                        {
                            if(dFeatures[2] <= 0.07)
                            {
                                if(dFeatures[0] <= 140.45)
                                {
                                    return 0;
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                if(dFeatures[7] <= 0.38)
                                {
                                    return 0;
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 96.79)
                            {
                                if(dFeatures[2] <= 0.17)
                                {
                                    return 0;
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[2] <= 1.15)
                    {
                        if(dFeatures[0] <= 17.01)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[0] <= 220.38)
                            {
                                return 0;
                            }
                            else
                            {
                                if(dFeatures[0] <= 606.92)
                                {
                                    return 1;
                                }
                                else
                                {
                                    if(dFeatures[0] <= 888.94)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[9] <= 0.38)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[8] <= 0.12)
        {
            if(dFeatures[3] <= 0.00)
            {
                return 0;
            }
            else
            {
                if(dFeatures[6] <= 0.38)
                {
                    if(dFeatures[1] <= 2.41)
                    {
                        if(dFeatures[2] <= 0.85)
                        {
                            if(dFeatures[1] <= 1.95)
                            {
                                if(dFeatures[2] <= 0.05)
                                {
                                    if(dFeatures[1] <= 1.30)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[0] <= 0.13)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[2] <= 0.00)
                                {
                                    return 0;
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[3] <= 0.00)
                        {
                            if(dFeatures[0] <= 17.02)
                            {
                                return 0;
                            }
                            else
                            {
                                if(dFeatures[7] <= 0.88)
                                {
                                    return 0;
                                }
                                else
                                {
                                    if(dFeatures[1] <= 2131.00)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(dFeatures[3] <= 0.01)
                            {
                                return 1;
                            }
                            else
                            {
                                if(dFeatures[3] <= 2.40)
                                {
                                    if(dFeatures[2] <= 0.86)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[0] <= 1.14)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[0] <= 5.19)
                    {
                        if(dFeatures[3] <= 1.80)
                        {
                            if(dFeatures[2] <= 0.29)
                            {
                                if(dFeatures[0] <= 0.52)
                                {
                                    if(dFeatures[0] <= 0.49)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[2] <= 0.00)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[3] <= 0.29)
                                {
                                    return 1;
                                }
                                else
                                {
                                    if(dFeatures[2] <= 0.93)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[9] <= 0.12)
                        {
                            if(dFeatures[2] <= 0.29)
                            {
                                if(dFeatures[2] <= 0.01)
                                {
                                    if(dFeatures[2] <= 0.01)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                if(dFeatures[1] <= 26.67)
                                {
                                    if(dFeatures[4] <= 1.00)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[0] <= 311.52)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 196.77)
                            {
                                if(dFeatures[0] <= 31.67)
                                {
                                    return 0;
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[2] <= 0.00)
            {
                return 1;
            }
            else
            {
                if(dFeatures[2] <= 1.39)
                {
                    if(dFeatures[0] <= 0.41)
                    {
                        if(dFeatures[3] <= 0.09)
                        {
                            if(dFeatures[2] <= 0.03)
                            {
                                if(dFeatures[0] <= 0.26)
                                {
                                    if(dFeatures[0] <= 0.13)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[0] <= 0.35)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.05)
                            {
                                return 1;
                            }
                            else
                            {
                                if(dFeatures[7] <= 0.62)
                                {
                                    if(dFeatures[3] <= 0.47)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[3] <= 0.37)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[7] <= 0.62)
                        {
                            if(dFeatures[0] <= 18.85)
                            {
                                if(dFeatures[2] <= 0.01)
                                {
                                    if(dFeatures[2] <= 0.00)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[9] <= 0.12)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[1] <= 26.05)
                                {
                                    return 1;
                                }
                                else
                                {
                                    if(dFeatures[7] <= 0.12)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(dFeatures[3] <= 0.27)
                            {
                                if(dFeatures[3] <= 0.21)
                                {
                                    if(dFeatures[0] <= 0.57)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[3] <= 1.29)
                    {
                        return 1;
                    }
                    else
                    {
                        if(dFeatures[3] <= 2.07)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[7] <= 0.62)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }
}
else
{
    if(dFeatures[8] <= 0.12)
    {
        if(dFeatures[9] <= 0.12)
        {
            if(dFeatures[4] <= 15.50)
            {
                if(dFeatures[0] <= 2.84)
                {
                    if(dFeatures[7] <= 0.38)
                    {
                        if(dFeatures[2] <= 0.01)
                        {
                            if(dFeatures[5] <= 11.50)
                            {
                                if(dFeatures[0] <= 0.42)
                                {
                                    if(dFeatures[2] <= 0.00)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[3] <= 0.00)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[2] <= 0.00)
                                {
                                    return 1;
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                        else
                        {
                            if(dFeatures[5] <= 7.50)
                            {
                                if(dFeatures[0] <= 0.06)
                                {
                                    return 0;
                                }
                                else
                                {
                                    if(dFeatures[0] <= 0.74)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[0] <= 0.21)
                                {
                                    return 0;
                                }
                                else
                                {
                                    if(dFeatures[1] <= 1.30)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[3] <= 0.00)
                        {
                            if(dFeatures[0] <= 0.17)
                            {
                                if(dFeatures[2] <= 0.00)
                                {
                                    if(dFeatures[2] <= 0.00)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[2] <= 0.02)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[1] <= 2.52)
                                {
                                    if(dFeatures[4] <= 12.50)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 1.49)
                            {
                                if(dFeatures[3] <= 0.07)
                                {
                                    if(dFeatures[0] <= 0.47)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[0] <= 0.35)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[1] <= 2.41)
                                {
                                    return 1;
                                }
                                else
                                {
                                    if(dFeatures[3] <= 0.06)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[0] <= 96.06)
                    {
                        if(dFeatures[5] <= 5.50)
                        {
                            if(dFeatures[3] <= 0.03)
                            {
                                if(dFeatures[3] <= 0.01)
                                {
                                    if(dFeatures[3] <= 0.01)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                            else
                            {
                                if(dFeatures[2] <= 0.09)
                                {
                                    if(dFeatures[2] <= 0.02)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[7] <= 0.88)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(dFeatures[3] <= 1.10)
                            {
                                if(dFeatures[0] <= 20.83)
                                {
                                    return 0;
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                            else
                            {
                                if(dFeatures[2] <= 1.08)
                                {
                                    return 1;
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[3] <= 1.73)
                        {
                            if(dFeatures[4] <= 8.50)
                            {
                                if(dFeatures[3] <= 0.00)
                                {
                                    if(dFeatures[7] <= 0.62)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                if(dFeatures[2] <= 0.28)
                                {
                                    return 0;
                                }
                                else
                                {
                                    if(dFeatures[3] <= 0.00)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[4] <= 32.50)
                {
                    if(dFeatures[2] <= 0.35)
                    {
                        if(dFeatures[4] <= 29.50)
                        {
                            if(dFeatures[2] <= 0.02)
                            {
                                if(dFeatures[2] <= 0.02)
                                {
                                    if(dFeatures[5] <= 8.50)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[4] <= 22.50)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[1] <= 39.49)
                                {
                                    if(dFeatures[5] <= 6.50)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                        }
                        else
                        {
                            if(dFeatures[1] <= 1.96)
                            {
                                if(dFeatures[0] <= 0.21)
                                {
                                    return 1;
                                }
                                else
                                {
                                    if(dFeatures[5] <= 16.50)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 0.48)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[0] <= 453.82)
                            {
                                if(dFeatures[0] <= 3.12)
                                {
                                    if(dFeatures[6] <= 0.38)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[4] <= 17.50)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[1] <= 104.05)
                    {
                        if(dFeatures[4] <= 40.50)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[5] <= 20.50)
                            {
                                if(dFeatures[0] <= 1.52)
                                {
                                    return 0;
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                if(dFeatures[0] <= 0.34)
                                {
                                    if(dFeatures[0] <= 0.23)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 472.98)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[5] <= 19.00)
            {
                if(dFeatures[3] <= 0.00)
                {
                    if(dFeatures[0] <= 18.79)
                    {
                        if(dFeatures[0] <= 12.84)
                        {
                            if(dFeatures[2] <= 0.01)
                            {
                                if(dFeatures[2] <= 0.00)
                                {
                                    if(dFeatures[6] <= 0.38)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[2] <= 0.01)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 5.78)
                    {
                        return 0;
                    }
                    else
                    {
                        if(dFeatures[1] <= 1283.18)
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 0;
                            }
                            else
                            {
                                if(dFeatures[1] <= 471.23)
                                {
                                    if(dFeatures[5] <= 8.50)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[4] <= 10.50)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
            else
            {
                return 0;
            }
        }
    }
    else
    {
        if(dFeatures[5] <= 10.50)
        {
            if(dFeatures[6] <= 0.38)
            {
                if(dFeatures[1] <= 1.39)
                {
                    if(dFeatures[1] <= 0.79)
                    {
                        if(dFeatures[0] <= 0.15)
                        {
                            if(dFeatures[5] <= 5.50)
                            {
                                if(dFeatures[1] <= 0.34)
                                {
                                    return 0;
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[7] <= 0.62)
                            {
                                if(dFeatures[0] <= 0.83)
                                {
                                    if(dFeatures[4] <= 11.50)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                if(dFeatures[2] <= 0.00)
                                {
                                    if(dFeatures[5] <= 5.00)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[5] <= 9.50)
                        {
                            if(dFeatures[4] <= 15.50)
                            {
                                if(dFeatures[0] <= 0.36)
                                {
                                    return 1;
                                }
                                else
                                {
                                    if(dFeatures[1] <= 0.81)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    if(dFeatures[8] <= 0.38)
                    {
                        if(dFeatures[0] <= 325.47)
                        {
                            if(dFeatures[2] <= 0.07)
                            {
                                if(dFeatures[2] <= 0.00)
                                {
                                    if(dFeatures[4] <= 6.00)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[2] <= 0.01)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[2] <= 0.11)
                                {
                                    if(dFeatures[1] <= 5.77)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[1] <= 3.90)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 15.23)
                        {
                            if(dFeatures[1] <= 29.52)
                            {
                                if(dFeatures[4] <= 13.50)
                                {
                                    if(dFeatures[2] <= 0.00)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[7] <= 0.38)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 0.46)
                {
                    if(dFeatures[2] <= 0.02)
                    {
                        if(dFeatures[2] <= 0.01)
                        {
                            if(dFeatures[5] <= 7.00)
                            {
                                if(dFeatures[2] <= 0.00)
                                {
                                    return 0;
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[4] <= 16.00)
                        {
                            if(dFeatures[3] <= 0.05)
                            {
                                if(dFeatures[5] <= 9.00)
                                {
                                    return 0;
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    if(dFeatures[1] <= 1.77)
                    {
                        return 0;
                    }
                    else
                    {
                        if(dFeatures[1] <= 2.10)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[1] <= 13.61)
                            {
                                if(dFeatures[0] <= 3.77)
                                {
                                    if(dFeatures[3] <= 0.24)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                            else
                            {
                                if(dFeatures[1] <= 186.01)
                                {
                                    if(dFeatures[3] <= 0.22)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[8] <= 0.38)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[2] <= 0.00)
            {
                if(dFeatures[5] <= 22.00)
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if(dFeatures[0] <= 2.16)
                {
                    if(dFeatures[6] <= 0.38)
                    {
                        if(dFeatures[1] <= 1.07)
                        {
                            if(dFeatures[3] <= 0.00)
                            {
                                return 1;
                            }
                            else
                            {
                                if(dFeatures[2] <= 0.00)
                                {
                                    if(dFeatures[1] <= 0.86)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[0] <= 0.38)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 0.42)
                            {
                                return 1;
                            }
                            else
                            {
                                if(dFeatures[4] <= 42.50)
                                {
                                    if(dFeatures[1] <= 6.39)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[0] <= 0.68)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 1.54)
                        {
                            if(dFeatures[4] <= 14.50)
                            {
                                return 0;
                            }
                            else
                            {
                                if(dFeatures[5] <= 15.50)
                                {
                                    if(dFeatures[2] <= 0.26)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[3] <= 0.01)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    if(dFeatures[3] <= 2.30)
                    {
                        if(dFeatures[0] <= 10.40)
                        {
                            if(dFeatures[0] <= 9.81)
                            {
                                if(dFeatures[3] <= 0.19)
                                {
                                    if(dFeatures[1] <= 2.24)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[4] <= 61.50)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[4] <= 52.00)
                                {
                                    return 1;
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                if(dFeatures[1] <= 46.40)
                                {
                                    return 0;
                                }
                                else
                                {
                                    if(dFeatures[3] <= 0.00)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                            else
                            {
                                if(dFeatures[5] <= 75.50)
                                {
                                    if(dFeatures[5] <= 39.50)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    if(dFeatures[0] <= 403.95)
                                    {
                                        return 0;
                                    }
                                    else
                                    {
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 162.54)
                        {
                            if(dFeatures[1] <= 55.30)
                            {
                                return 0;
                            }
                            else
                            {
                                if(dFeatures[2] <= 2.50)
                                {
                                    return 1;
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                        else
                        {
                            return 0;
                            }
                        }
                    }
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth1_QP29_v3(Double *dFeatures)
{
if(dFeatures[1] <= 0.47)
{
    if(dFeatures[4] <= 16.50)
    {
        if(dFeatures[7] <= 0.12)
        {
            if(dFeatures[3] <= 0.04)
            {
                if(dFeatures[2] <= 0.58)
                {
                    if(dFeatures[9] <= 0.38)
                    {
                        if(dFeatures[2] <= 0.18)
                        {
                            if(dFeatures[0] <= 0.02)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.21)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[5] <= 4.00)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.70)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[1] <= 0.30)
                {
                    if(dFeatures[3] <= 0.11)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
        else
        {
            if(dFeatures[4] <= 9.50)
            {
                if(dFeatures[0] <= 0.23)
                {
                    if(dFeatures[9] <= 0.38)
                    {
                        if(dFeatures[2] <= 0.29)
                        {
                            if(dFeatures[3] <= 0.05)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[6] <= 0.12)
                    {
                        if(dFeatures[3] <= 0.00)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 0.23)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[6] <= 0.62)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 0.16)
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        if(dFeatures[4] <= 11.50)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 0.18)
                    {
                        if(dFeatures[1] <= 0.33)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[5] <= 7.50)
                        {
                            if(dFeatures[8] <= 0.12)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[0] <= 1.23)
        {
            if(dFeatures[7] <= 0.38)
            {
                return 1;
            }
            else
            {
                if(dFeatures[0] <= 0.35)
                {
                    if(dFeatures[8] <= 0.12)
                    {
                        if(dFeatures[9] <= 0.25)
                        {
                            if(dFeatures[0] <= 0.23)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[3] <= 0.03)
                        {
                            if(dFeatures[0] <= 0.32)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    if(dFeatures[3] <= 0.05)
                    {
                        if(dFeatures[4] <= 19.50)
                        {
                            if(dFeatures[7] <= 0.62)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.01)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.24)
                        {
                            if(dFeatures[0] <= 0.35)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[7] <= 0.38)
            {
                return 1;
            }
            else
            {
                if(dFeatures[6] <= 0.12)
                {
                    return 0;
                }
                else
                {
                    if(dFeatures[2] <= 0.05)
                    {
                        if(dFeatures[4] <= 20.50)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.04)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
    }
}
else
{
    if(dFeatures[5] <= 9.50)
    {
        if(dFeatures[8] <= 0.12)
        {
            if(dFeatures[0] <= 0.66)
            {
                if(dFeatures[2] <= 0.00)
                {
                    if(dFeatures[7] <= 0.88)
                    {
                        if(dFeatures[3] <= 0.00)
                        {
                            if(dFeatures[5] <= 2.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[5] <= 8.50)
                    {
                        if(dFeatures[0] <= 0.55)
                        {
                            if(dFeatures[0] <= 0.54)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.10)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[3] <= 0.00)
                {
                    if(dFeatures[5] <= 8.50)
                    {
                        if(dFeatures[0] <= 1.84)
                        {
                            if(dFeatures[0] <= 1.36)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[6] <= 0.38)
                    {
                        if(dFeatures[0] <= 2.53)
                        {
                            if(dFeatures[1] <= 3.01)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[3] <= 1.24)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[3] <= 0.04)
                        {
                            if(dFeatures[0] <= 1.24)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[1] <= 6.67)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[0] <= 0.94)
            {
                if(dFeatures[5] <= 1.00)
                {
                    if(dFeatures[8] <= 0.38)
                    {
                        if(dFeatures[2] <= 0.04)
                        {
                            if(dFeatures[1] <= 1.19)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[7] <= 0.38)
                        {
                            if(dFeatures[2] <= 0.16)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    if(dFeatures[0] <= 0.88)
                    {
                        if(dFeatures[4] <= 15.50)
                        {
                            if(dFeatures[1] <= 0.55)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[2] <= 0.00)
                {
                    if(dFeatures[1] <= 200.80)
                    {
                        if(dFeatures[5] <= 5.50)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[6] <= 0.62)
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[4] <= 6.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[3] <= 2.27)
                        {
                            if(dFeatures[2] <= 0.37)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[4] <= 33.50)
        {
            if(dFeatures[8] <= 0.12)
            {
                if(dFeatures[5] <= 12.50)
                {
                    if(dFeatures[2] <= 0.05)
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[0] <= 0.41)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 23.32)
                        {
                            if(dFeatures[2] <= 0.16)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    if(dFeatures[3] <= 0.00)
                    {
                        return 1;
                    }
                    else
                    {
                        if(dFeatures[3] <= 1.01)
                        {
                            if(dFeatures[0] <= 1.01)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 1.28)
                {
                    if(dFeatures[2] <= 0.04)
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            if(dFeatures[5] <= 14.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 1.19)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.37)
                        {
                            if(dFeatures[1] <= 0.76)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    if(dFeatures[0] <= 2.25)
                    {
                        if(dFeatures[2] <= 0.17)
                        {
                            if(dFeatures[3] <= 0.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 1.55)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 1722.25)
                        {
                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[0] <= 1.01)
            {
                if(dFeatures[1] <= 1.42)
                {
                    if(dFeatures[2] <= 0.02)
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            if(dFeatures[6] <= 0.12)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.06)
                        {
                            if(dFeatures[3] <= 0.07)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    if(dFeatures[4] <= 38.50)
                    {
                        return 1;
                    }
                    else
                    {
                        if(dFeatures[5] <= 29.50)
                        {
                            if(dFeatures[2] <= 0.08)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[4] <= 75.50)
                {
                    if(dFeatures[1] <= 2.59)
                    {
                        if(dFeatures[5] <= 40.00)
                        {
                            if(dFeatures[3] <= 0.36)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 6.29)
                        {
                            if(dFeatures[3] <= 0.01)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[6] <= 0.38)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[3] <= 0.00)
                    {
                        return 1;
                    }
                    else
                    {
                        if(dFeatures[4] <= 178.50)
                        {
                            if(dFeatures[4] <= 163.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth1_QP30_v3(Double *dFeatures)
{
if(dFeatures[1] <= 0.57)
{
    if(dFeatures[5] <= 6.50)
    {
        if(dFeatures[6] <= 0.62)
        {
            if(dFeatures[0] <= 0.27)
            {
                if(dFeatures[2] <= 0.26)
                {
                    return 1;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if(dFeatures[3] <= 0.00)
                {
                    if(dFeatures[2] <= 2.15)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.01)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[0] <= 1624.72)
            {
                if(dFeatures[7] <= 0.12)
                {
                    return 1;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                return 0;
            }
        }
    }
    else
    {
        if(dFeatures[0] <= 0.36)
        {
            if(dFeatures[4] <= 33.50)
            {
                if(dFeatures[6] <= 0.62)
                {
                    return 1;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if(dFeatures[3] <= 0.00)
                {
                    if(dFeatures[3] <= 0.00)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.03)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[5] <= 17.50)
            {
                if(dFeatures[2] <= 0.00)
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 94.20)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 2.19)
                {
                    if(dFeatures[3] <= 0.02)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.63)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
    }
}
else
{
    if(dFeatures[5] <= 10.50)
    {
        if(dFeatures[8] <= 0.12)
        {
            if(dFeatures[6] <= 0.38)
            {
                if(dFeatures[3] <= 0.75)
                {
                    if(dFeatures[0] <= 0.77)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[3] <= 0.79)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[4] <= 9.50)
                {
                    if(dFeatures[2] <= 1.85)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[5] <= 8.50)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[0] <= 5.86)
            {
                if(dFeatures[2] <= 0.01)
                {
                    if(dFeatures[5] <= 2.50)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 0.53)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[3] <= 0.33)
                {
                    if(dFeatures[2] <= 0.11)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 162.72)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[0] <= 2.04)
        {
            if(dFeatures[0] <= 0.94)
            {
                if(dFeatures[5] <= 19.50)
                {
                    if(dFeatures[3] <= 0.01)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 0.75)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[4] <= 78.00)
                {
                    if(dFeatures[1] <= 2.43)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 0;
                }
            }
        }
        else
        {
            if(dFeatures[4] <= 34.50)
            {
                if(dFeatures[8] <= 0.12)
                {
                    if(dFeatures[1] <= 41.95)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 5.73)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 10.81)
                {
                    if(dFeatures[2] <= 0.11)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[9] <= 0.62)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                        }
                    }
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth1_QP33_v3(Double *dFeatures)
{
if(dFeatures[1] <= 0.23)
{
    if(dFeatures[7] <= 0.12)
    {
        if(dFeatures[2] <= 0.00)
        {
            if(dFeatures[0] <= 5.21)
            {
                if(dFeatures[0] <= 0.10)
                {
                    if(dFeatures[4] <= 12.50)
                    {
                        if(dFeatures[4] <= 8.50)
                        {
                            if(dFeatures[9] <= 0.62)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[9] <= 0.38)
                        {
                            return 0;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if(dFeatures[0] <= 9.40)
                {
                    return 0;
                }
                else
                {
                    if(dFeatures[0] <= 18.59)
                    {
                        if(dFeatures[0] <= 13.46)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[2] <= 0.00)
            {
                if(dFeatures[2] <= 0.00)
                {
                    return 0;
                }
                else
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[2] <= 0.03)
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        if(dFeatures[0] <= 1.67)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[0] <= 2.29)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.01)
                        {
                            if(dFeatures[2] <= 0.01)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.03)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.56)
                    {
                        if(dFeatures[0] <= 6.55)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[0] <= 11.50)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.74)
                        {
                            if(dFeatures[0] <= 0.63)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[4] <= 1.50)
        {
            if(dFeatures[6] <= 0.38)
            {
                if(dFeatures[0] <= 0.26)
                {
                    if(dFeatures[9] <= 0.62)
                    {
                        if(dFeatures[6] <= 0.12)
                        {
                            return 0;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 150.04)
                    {
                        if(dFeatures[0] <= 5.52)
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.91)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 1.01)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[2] <= 1.79)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 36.67)
                {
                    if(dFeatures[9] <= 0.12)
                    {
                        if(dFeatures[0] <= 36.58)
                        {
                            if(dFeatures[2] <= 0.24)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.33)
                        {
                            if(dFeatures[2] <= 0.01)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    if(dFeatures[0] <= 358.30)
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 178.38)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[9] <= 0.12)
                        {
                            if(dFeatures[0] <= 605.02)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[2] <= 0.00)
            {
                if(dFeatures[6] <= 0.38)
                {
                    if(dFeatures[5] <= 4.00)
                    {
                        return 0;
                    }
                    else
                    {
                        if(dFeatures[0] <= 1.62)
                        {
                            if(dFeatures[0] <= 0.22)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    if(dFeatures[5] <= 16.50)
                    {
                        if(dFeatures[4] <= 9.50)
                        {
                            if(dFeatures[4] <= 6.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 193.35)
                {
                    if(dFeatures[2] <= 1.32)
                    {
                        if(dFeatures[0] <= 1.72)
                        {
                            if(dFeatures[2] <= 0.15)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.10)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
    }
}
else
{
    if(dFeatures[4] <= 3.50)
    {
        if(dFeatures[8] <= 0.12)
        {
            if(dFeatures[1] <= 32.32)
            {
                if(dFeatures[0] <= 0.34)
                {
                    if(dFeatures[2] <= 0.01)
                    {
                        if(dFeatures[0] <= 0.27)
                        {
                            if(dFeatures[2] <= 0.01)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[3] <= 0.02)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.52)
                        {
                            if(dFeatures[4] <= 1.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    if(dFeatures[7] <= 0.62)
                    {
                        if(dFeatures[3] <= 2.08)
                        {
                            if(dFeatures[2] <= 0.72)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.64)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.01)
                        {
                            if(dFeatures[1] <= 2.52)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 1.74)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 685.58)
                {
                    if(dFeatures[6] <= 0.38)
                    {
                        if(dFeatures[0] <= 18.62)
                        {
                            return 0;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 37.51)
                        {
                            if(dFeatures[3] <= 1.99)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 107.49)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[3] <= 1.56)
                    {
                        if(dFeatures[7] <= 0.38)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[6] <= 0.38)
            {
                if(dFeatures[1] <= 4.95)
                {
                    if(dFeatures[1] <= 2.05)
                    {
                        if(dFeatures[7] <= 0.62)
                        {
                            if(dFeatures[1] <= 1.32)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 2.90)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[3] <= 0.12)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[1] <= 10.33)
                    {
                        if(dFeatures[1] <= 8.32)
                        {
                            if(dFeatures[1] <= 5.89)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[3] <= 0.56)
                        {
                            if(dFeatures[6] <= 0.12)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[7] <= 0.62)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[3] <= 2.13)
                {
                    if(dFeatures[2] <= 0.45)
                    {
                        if(dFeatures[1] <= 23.99)
                        {
                            if(dFeatures[2] <= 0.26)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[3] <= 0.46)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.88)
                        {
                            if(dFeatures[4] <= 2.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 1.05)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    return 0;
                }
            }
        }
    }
    else
    {
        if(dFeatures[4] <= 12.50)
        {
            if(dFeatures[3] <= 1.36)
            {
                if(dFeatures[3] <= 0.34)
                {
                    if(dFeatures[1] <= 203.35)
                    {
                        if(dFeatures[3] <= 0.03)
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[4] <= 9.50)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[5] <= 5.50)
                        {
                            if(dFeatures[3] <= 0.13)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.42)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[1] <= 215.83)
                    {
                        if(dFeatures[1] <= 11.65)
                        {
                            if(dFeatures[8] <= 0.12)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 5.48)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[8] <= 0.62)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[6] <= 0.12)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[2] <= 1.20)
                {
                    if(dFeatures[2] <= 0.75)
                    {
                        if(dFeatures[0] <= 954.98)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 22.30)
                    {
                        if(dFeatures[8] <= 0.12)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 106.68)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[3] <= 2.43)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[1] <= 458.49)
            {
                if(dFeatures[5] <= 11.50)
                {
                    if(dFeatures[0] <= 0.25)
                    {
                        return 1;
                    }
                    else
                    {
                        if(dFeatures[3] <= 0.54)
                        {
                            if(dFeatures[6] <= 0.38)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[3] <= 1.21)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[1] <= 400.67)
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[3] <= 0.01)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                return 0;
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth1_QP34_v3(Double *dFeatures)
{
if(dFeatures[8] <= 0.12)
{
    if(dFeatures[7] <= 0.38)
    {
        if(dFeatures[0] <= 11.36)
        {
            if(dFeatures[4] <= 28.50)
            {
                if(dFeatures[6] <= 0.62)
                {
                    return 0;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[0] <= 0.53)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
        }
        else
        {
            if(dFeatures[0] <= 14.18)
            {
                if(dFeatures[0] <= 13.64)
                {
                    if(dFeatures[1] <= 32.27)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[1] <= 2992.28)
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[4] <= 6.00)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[5] <= 11.50)
        {
            if(dFeatures[0] <= 2.02)
            {
                if(dFeatures[2] <= 0.00)
                {
                    return 0;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(dFeatures[6] <= 0.38)
                {
                    if(dFeatures[4] <= 21.50)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 5.78)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[0] <= 28.97)
            {
                if(dFeatures[5] <= 12.50)
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 0.67)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 508.97)
                {
                    if(dFeatures[2] <= 0.24)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    return 0;
                }
            }
        }
    }
}
else
{
    if(dFeatures[4] <= 29.50)
    {
        if(dFeatures[8] <= 0.38)
        {
            if(dFeatures[3] <= 0.00)
            {
                if(dFeatures[7] <= 0.38)
                {
                    if(dFeatures[5] <= 6.50)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 0.67)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[1] <= 4.27)
                {
                    if(dFeatures[9] <= 0.12)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 268.71)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[3] <= 1.47)
            {
                if(dFeatures[2] <= 0.43)
                {
                    if(dFeatures[3] <= 0.56)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 1;
            }
        }
    }
    else
    {
        if(dFeatures[1] <= 234.83)
        {
            return 0;
        }
        else
        {
            if(dFeatures[1] <= 450.60)
            {
                return 1;
            }
            else
            {
                return 0;
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth1_QP35_v3(Double *dFeatures)
{
if(dFeatures[1] <= 2.64)
{
    if(dFeatures[6] <= 0.62)
    {
        if(dFeatures[4] <= 18.50)
        {
            if(dFeatures[6] <= 0.12)
            {
                if(dFeatures[0] <= 0.31)
                {
                    return 1;
                }
                else
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            if(dFeatures[0] <= 1.76)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 4.02)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 0.32)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[4] <= 15.50)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[2] <= 0.13)
                {
                    if(dFeatures[3] <= 0.00)
                    {
                        if(dFeatures[0] <= 1.28)
                        {
                            if(dFeatures[2] <= 0.11)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 1.30)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 1.02)
                        {
                            return 0;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.14)
                    {
                        if(dFeatures[4] <= 2.50)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 2.40)
                        {
                            return 0;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[5] <= 16.50)
            {
                if(dFeatures[9] <= 0.38)
                {
                    if(dFeatures[9] <= 0.12)
                    {
                        if(dFeatures[8] <= 0.62)
                        {
                            if(dFeatures[4] <= 25.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.52)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if(dFeatures[6] <= 0.12)
                {
                    if(dFeatures[4] <= 46.00)
                    {
                        if(dFeatures[0] <= 0.44)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.01)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.86)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[1] <= 0.63)
        {
            if(dFeatures[2] <= 0.49)
            {
                if(dFeatures[6] <= 0.88)
                {
                    if(dFeatures[0] <= 0.85)
                    {
                        if(dFeatures[0] <= 0.53)
                        {
                            return 0;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 42.49)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[0] <= 46.52)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if(dFeatures[2] <= 0.53)
                {
                    if(dFeatures[4] <= 1.50)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.90)
                    {
                        if(dFeatures[2] <= 0.89)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[1] <= 0.84)
            {
                return 0;
            }
            else
            {
                if(dFeatures[2] <= 0.00)
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[4] <= 6.50)
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 2.41)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
        }
    }
}
else
{
    if(dFeatures[5] <= 11.50)
    {
        if(dFeatures[8] <= 0.12)
        {
            if(dFeatures[0] <= 3.56)
            {
                if(dFeatures[7] <= 0.88)
                {
                    if(dFeatures[0] <= 1.54)
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 6.67)
                        {
                            return 0;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    if(dFeatures[0] <= 1.75)
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[4] <= 6.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            if(dFeatures[3] <= 0.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 2.45)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[7] <= 0.38)
                {
                    if(dFeatures[0] <= 25.56)
                    {
                        if(dFeatures[3] <= 1.68)
                        {
                            if(dFeatures[0] <= 25.10)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.73)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[4] <= 2.50)
                    {
                        if(dFeatures[0] <= 201.21)
                        {
                            if(dFeatures[3] <= 0.00)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 205.67)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 1.81)
                        {
                            if(dFeatures[3] <= 0.00)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[5] <= 6.50)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[1] <= 22.95)
            {
                if(dFeatures[3] <= 0.00)
                {
                    if(dFeatures[3] <= 0.00)
                    {
                        if(dFeatures[5] <= 8.50)
                        {
                            if(dFeatures[5] <= 6.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[3] <= 0.01)
                    {
                        if(dFeatures[4] <= 4.00)
                        {
                            return 0;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 2.71)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[3] <= 1.77)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[2] <= 0.87)
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        return 1;
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.01)
                        {
                            if(dFeatures[1] <= 120.64)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            if(dFeatures[8] <= 0.38)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[1] <= 29.67)
                    {
                        if(dFeatures[6] <= 0.38)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[3] <= 1.52)
                        {
                            return 0;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[1] <= 65.32)
        {
            if(dFeatures[0] <= 6.21)
            {
                if(dFeatures[1] <= 7.57)
                {
                    if(dFeatures[2] <= 0.11)
                    {
                        if(dFeatures[0] <= 4.48)
                        {
                            if(dFeatures[0] <= 3.06)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[5] <= 25.00)
                    {
                        if(dFeatures[2] <= 0.09)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[7] <= 0.75)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 4.33)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[2] <= 0.01)
                {
                    if(dFeatures[4] <= 15.00)
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[1] <= 15.87)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[7] <= 0.88)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[3] <= 0.00)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[4] <= 20.50)
                    {
                        if(dFeatures[1] <= 9.51)
                        {
                            if(dFeatures[7] <= 0.88)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 1.41)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 45.42)
                        {
                            if(dFeatures[5] <= 12.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 195.29)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[9] <= 0.12)
            {
                if(dFeatures[0] <= 26.75)
                {
                    return 1;
                }
                else
                {
                    if(dFeatures[1] <= 4464.07)
                    {
                        if(dFeatures[7] <= 0.12)
                        {
                            if(dFeatures[4] <= 46.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[4] <= 14.50)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[4] <= 37.50)
                {
                    if(dFeatures[3] <= 0.06)
                    {
                        if(dFeatures[1] <= 142.57)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[8] <= 0.38)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[1] <= 477.68)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    return 0;
                    }
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth1_QP38_v3(Double *dFeatures)
{
if(dFeatures[3] <= 0.00)
{
    if(dFeatures[7] <= 0.12)
    {
        if(dFeatures[6] <= 0.38)
        {
            if(dFeatures[4] <= 8.00)
            {
                return 0;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if(dFeatures[0] <= 13.12)
            {
                if(dFeatures[0] <= 0.10)
                {
                    if(dFeatures[4] <= 5.00)
                    {
                        return 0;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[0] <= 0.24)
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 0.34)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 14.16)
                {
                    return 0;
                }
                else
                {
                    if(dFeatures[2] <= 0.54)
                    {
                        if(dFeatures[2] <= 0.00)
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 21.21)
                        {
                            if(dFeatures[0] <= 17.45)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[4] <= 1.50)
        {
            if(dFeatures[0] <= 0.93)
            {
                if(dFeatures[1] <= 0.32)
                {
                    if(dFeatures[6] <= 0.38)
                    {
                        if(dFeatures[0] <= 0.29)
                        {
                            if(dFeatures[2] <= 0.09)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.01)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 0.23)
                        {
                            return 0;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    if(dFeatures[0] <= 0.25)
                    {
                        return 1;
                    }
                    else
                    {
                        if(dFeatures[0] <= 0.50)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 0.96)
                {
                    return 0;
                }
                else
                {
                    if(dFeatures[0] <= 23.31)
                    {
                        if(dFeatures[0] <= 4.82)
                        {
                            if(dFeatures[0] <= 1.76)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 6.20)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[6] <= 0.38)
                        {
                            return 0;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
        }
        else
        {
            if(dFeatures[0] <= 7.97)
            {
                if(dFeatures[0] <= 1.28)
                {
                    if(dFeatures[4] <= 4.50)
                    {
                        if(dFeatures[6] <= 0.38)
                        {
                            return 0;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[5] <= 14.00)
                        {
                            if(dFeatures[0] <= 1.04)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    if(dFeatures[6] <= 0.62)
                    {
                        if(dFeatures[6] <= 0.12)
                        {
                            if(dFeatures[5] <= 8.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.03)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[5] <= 5.50)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[6] <= 0.38)
                {
                    if(dFeatures[0] <= 10.79)
                    {
                        return 1;
                    }
                    else
                    {
                        if(dFeatures[5] <= 15.50)
                        {
                            if(dFeatures[5] <= 6.50)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 70.34)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[5] <= 16.50)
                    {
                        if(dFeatures[6] <= 0.62)
                        {
                            if(dFeatures[0] <= 1237.54)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[2] <= 0.37)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
    }
}
else
{
    if(dFeatures[4] <= 9.50)
    {
        if(dFeatures[6] <= 0.12)
        {
            if(dFeatures[0] <= 25.67)
            {
                if(dFeatures[2] <= 0.32)
                {
                    if(dFeatures[2] <= 0.17)
                    {
                        if(dFeatures[3] <= 0.04)
                        {
                            if(dFeatures[2] <= 0.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if(dFeatures[5] <= 4.50)
                {
                    if(dFeatures[8] <= 0.38)
                    {
                        if(dFeatures[1] <= 2598.02)
                        {
                            if(dFeatures[2] <= 2.74)
                            {
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[8] <= 0.50)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[0] <= 239.56)
            {
                if(dFeatures[3] <= 2.70)
                {
                    if(dFeatures[8] <= 0.12)
                    {
                        if(dFeatures[3] <= 0.00)
                        {
                            if(dFeatures[7] <= 0.38)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[7] <= 0.62)
                            {
                                return 0;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if(dFeatures[0] <= 2.28)
                        {
                            if(dFeatures[3] <= 0.00)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if(dFeatures[0] <= 154.27)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
                else
                {
                    if(dFeatures[1] <= 172.91)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 2015.09)
                {
                    if(dFeatures[2] <= 0.35)
                    {
                        if(dFeatures[6] <= 0.38)
                        {
                            return 1;
                        }
                        else
                        {
                            if(dFeatures[3] <= 0.64)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    return 0;
                }
            }
        }
    }
    else
    {
        if(dFeatures[4] <= 15.50)
        {
            if(dFeatures[5] <= 5.50)
            {
                if(dFeatures[2] <= 0.28)
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if(dFeatures[0] <= 9.05)
                {
                    if(dFeatures[8] <= 0.12)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    return 0;
                }
            }
        }
        else
        {
            if(dFeatures[2] <= 0.11)
            {
                if(dFeatures[2] <= 0.00)
                {
                    return 0;
                }
                else
                {
                    if(dFeatures[5] <= 10.50)
                    {
                        if(dFeatures[1] <= 2.33)
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if(dFeatures[1] <= 2.41)
                        {
                            return 0;
                        }
                        else
                        {
                            if(dFeatures[3] <= 0.09)
                            {
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
            else
            {
                if(dFeatures[3] <= 0.65)
                {
                    return 0;
                }
                else
                {
                    if(dFeatures[4] <= 18.50)
                    {
                        if(dFeatures[1] <= 38.67)
                        {
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 0;
                        }
                    }
                }
            }
        }
    }
}

UInt TTrEngine::xdecide_depth1_QP39_v3(Double *dFeatures)
{
if(dFeatures[1] <= 4.27)
{
    return 1;
}
else
{
    return 1;
    }
}

UInt TTrEngine::xdecide_depth1_QP40_v3(Double *dFeatures)
{
if(dFeatures[1] <= 17.78)
{
    if(dFeatures[0] <= 2.65)
    {
        if(dFeatures[3] <= 0.00)
        {
            if(dFeatures[9] <= 0.38)
            {
                if(dFeatures[0] <= 1.02)
                {
                    return 1;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if(dFeatures[5] <= 12.50)
                {
                    if(dFeatures[4] <= 8.50)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
        else
        {
            if(dFeatures[4] <= 3.50)
            {
                if(dFeatures[3] <= 0.00)
                {
                    return 0;
                }
                else
                {
                    if(dFeatures[1] <= 6.10)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 1.02)
                {
                    if(dFeatures[0] <= 0.85)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    if(dFeatures[3] <= 0.05)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
    }
    else
    {
        if(dFeatures[0] <= 2.74)
        {
            if(dFeatures[2] <= 0.00)
            {
                if(dFeatures[0] <= 2.73)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if(dFeatures[9] <= 0.62)
            {
                if(dFeatures[2] <= 0.00)
                {
                    if(dFeatures[7] <= 0.38)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                return 0;
            }
        }
    }
}
else
{
    if(dFeatures[3] <= 0.08)
    {
        if(dFeatures[2] <= 0.01)
        {
            if(dFeatures[2] <= 0.01)
            {
                if(dFeatures[4] <= 0.50)
                {
                    if(dFeatures[3] <= 0.00)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[2] <= 0.00)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if(dFeatures[8] <= 0.38)
                {
                    return 1;
                }
                else
                {
                    if(dFeatures[1] <= 215.47)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[4] <= 18.50)
            {
                if(dFeatures[1] <= 52.27)
                {
                    if(dFeatures[1] <= 29.02)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 2015.75)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                return 0;
            }
        }
    }
    else
    {
        if(dFeatures[4] <= 19.50)
        {
            if(dFeatures[3] <= 0.30)
            {
                if(dFeatures[7] <= 0.12)
                {
                    return 0;
                }
                else
                {
                    if(dFeatures[7] <= 0.88)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                if(dFeatures[0] <= 29.52)
                {
                    if(dFeatures[1] <= 18.21)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if(dFeatures[1] <= 165.81)
                    {
                        return 1;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
        else
        {
            if(dFeatures[5] <= 11.00)
            {
                return 0;
            }
            else
            {
                if(dFeatures[0] <= 42.14)
                {
                    return 1;
                }
                else
                {
                    if(dFeatures[0] <= 256.46)
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                        }
                    }
                }
            }
        }
    }
}

