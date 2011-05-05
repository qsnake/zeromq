/*
    Copyright (c) 2007-2011 iMatix Corporation
    Copyright (c) 2007-2011 Other contributors as noted in the AUTHORS file

    This file is part of 0MQ.

    0MQ is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    0MQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../include/zmq.h"

#include "patch.hpp"
#include "err.hpp"

zmq::patch_t::patch_t (class ctx_t *parent_, uint32_t tid_) :
    socket_base_t (parent_, tid_),
    fq (this),
    dist (this)
{
    options.type = ZMQ_PATCH;
    options.requires_in = true;
    options.requires_out = true;
}

zmq::patch_t::~patch_t ()
{
}

void zmq::patch_t::xattach_pipes (class reader_t *inpipe_,
    class writer_t *outpipe_, const blob_t &peer_identity_)
{
    zmq_assert (inpipe_ && outpipe_);
    fq.attach (inpipe_);
    dist.attach (outpipe_);
}

void zmq::patch_t::process_term (int linger_)
{
    fq.terminate ();
    dist.terminate ();
    socket_base_t::process_term (linger_);
}

int zmq::patch_t::xsend (zmq_msg_t *msg_, int flags_)
{
    return dist.send (msg_, flags_);
}

int zmq::patch_t::xrecv (zmq_msg_t *msg_, int flags_)
{
    return fq.recv (msg_, flags_);
}

bool zmq::patch_t::xhas_in ()
{
    return fq.has_in ();
}

bool zmq::patch_t::xhas_out ()
{
    return dist.has_out ();
}
