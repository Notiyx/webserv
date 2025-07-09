/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InstanceInterface.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmetais <nmetais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:36:16 by tlonghin          #+#    #+#             */
/*   Updated: 2025/07/09 19:55:36 by nmetais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <InstanceInterface.hpp>
#include <Interface.hpp>

I_Location::~I_Location() {}
I_MethodsAllow::~I_MethodsAllow() {}
I_ErrorPage::~I_ErrorPage() {}
I_Listen::~I_Listen() {}
I_Host::~I_Host() {}
I_FolderList::~I_FolderList() {}
I_Client::~I_Client() {}


IS_MethodsAllow::IS_MethodsAllow() {}
IS_MethodsAllow::~IS_MethodsAllow() {}

IS_Location::IS_Location() {}
IS_Location::~IS_Location() {}

IS_ErrorPage::IS_ErrorPage() {}
IS_ErrorPage::~IS_ErrorPage() {}

IS_Listen::IS_Listen() {}
IS_Listen::~IS_Listen() {}

IS_Host::IS_Host() {}
IS_Host::~IS_Host() {}

IS_FolderList::IS_FolderList() {}
IS_FolderList::~IS_FolderList() {}

IS_Client::IS_Client(){
	buffer.clear();
	isHeader = false;
	isChunked = false;
	complete = false;
	content_length = 0;
}
IS_Client::~IS_Client() {}