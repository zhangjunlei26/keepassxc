/*
*  Copyright (C) 2010 Felix Geyer <debfx@fobos.de>
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 2 or (at your option)
*  version 3 of the License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "SymmetricCipherGcrypt.h"

SymmetricCipherGcrypt::~SymmetricCipherGcrypt()
{
    gcry_cipher_close(m_ctx);
}

void SymmetricCipherGcrypt::setAlgorithm(SymmetricCipher::Algorithm algo)
{
    switch (algo) {
    case SymmetricCipher::Aes256:
        m_algo = GCRY_CIPHER_AES256;
        break;

    default:
        Q_ASSERT(false);
        break;
    }
}

void SymmetricCipherGcrypt::setMode(SymmetricCipher::Mode mode)
{
    switch (mode) {
    case SymmetricCipher::Ecb:
        m_mode = GCRY_CIPHER_MODE_ECB;
        break;

    case SymmetricCipher::Cbc:
        m_mode = GCRY_CIPHER_MODE_CBC;
        break;

    default:
        Q_ASSERT(false);
        break;
    }
}

void SymmetricCipherGcrypt::setDirection(SymmetricCipher::Direction direction)
{
    m_direction = direction;
}

void SymmetricCipherGcrypt::init()
{
    gcry_error_t error;

    error = gcry_cipher_open(&m_ctx, m_algo, m_mode, 0);
    Q_ASSERT(error == 0); // TODO real error checking

    size_t blockSizeT;
    error = gcry_cipher_algo_info(m_algo, GCRYCTL_GET_BLKLEN, 0, &blockSizeT);
    Q_ASSERT(error == 0);
    m_blockSize = blockSizeT;
}

void SymmetricCipherGcrypt::setKey(const QByteArray& key)
{
    m_key = key;
    gcry_error_t error = gcry_cipher_setkey(m_ctx, m_key.constData(), m_key.size());
    Q_ASSERT(error == 0);
}

void SymmetricCipherGcrypt::setIv(const QByteArray& iv)
{
    m_iv = iv;
    gcry_error_t error = gcry_cipher_setiv(m_ctx, m_iv.constData(), m_iv.size());
    Q_ASSERT(error == 0);
}

QByteArray SymmetricCipherGcrypt::process(const QByteArray& data)
{
    // TODO check block size

    QByteArray result;
    result.resize(data.size());

    gcry_error_t error;

    if (m_direction == SymmetricCipher::Decrypt) {
        error = gcry_cipher_decrypt(m_ctx, result.data(), data.size(), data.constData(), data.size());
    }
    else {
        error = gcry_cipher_encrypt(m_ctx, result.data(), data.size(), data.constData(), data.size());
    }

    Q_ASSERT(error == 0);

    return result;
}

void SymmetricCipherGcrypt::processInPlace(QByteArray& data)
{
    // TODO check block size

    gcry_error_t error;

    if (m_direction == SymmetricCipher::Decrypt) {
        error = gcry_cipher_decrypt(m_ctx, data.data(), data.size(), 0, 0);
    }
    else {
        error = gcry_cipher_encrypt(m_ctx, data.data(), data.size(), 0, 0);
    }

    Q_ASSERT(error == 0);
}

void SymmetricCipherGcrypt::reset()
{
    gcry_error_t error;

    error = gcry_cipher_reset(m_ctx);
    Q_ASSERT(error == 0);
    error = gcry_cipher_setiv(m_ctx, m_iv.constData(), m_iv.size());
    Q_ASSERT(error == 0);
}

int SymmetricCipherGcrypt::blockSize() const
{
    return m_blockSize;
}