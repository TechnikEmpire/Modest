/*
 Copyright (C) 2016 Alexander Borisov
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 
 Author: lex.borisov@gmail.com (Alexander Borisov)
*/

#ifndef MyHTML_MyCSS_PROPERTY_SHARED_H
#define MyHTML_MyCSS_PROPERTY_SHARED_H
#pragma once

#include <mycss/property/myosi.h>
#include <mycss/values/consume.h>
#include <mycss/values/values.h>

#ifdef __cplusplus
extern "C" {
#endif

bool mycss_property_shared_switch_to_find_important(mycss_entry_t* entry);
bool mycss_property_shared_switch_to_parse_error(mycss_entry_t* entry);
bool mycss_property_shared_check_declaration_end(mycss_entry_t* entry, mycss_token_t* token);

bool mycss_property_shared_number(mycss_entry_t* entry, mycss_token_t* token, void** value, unsigned int* value_type, myhtml_string_t* str);
bool mycss_property_shared_length(mycss_entry_t* entry, mycss_token_t* token, void** value, unsigned int* value_type, myhtml_string_t* str);
bool mycss_property_shared_percentage(mycss_entry_t* entry, mycss_token_t* token, void** value, unsigned int* value_type, myhtml_string_t* str);
bool mycss_property_shared_length_percentage(mycss_entry_t* entry, mycss_token_t* token, void** value, unsigned int* value_type, myhtml_string_t* str);
bool mycss_property_shared_default(mycss_entry_t* entry, mycss_token_t* token, unsigned int* value_type, myhtml_string_t* str);
unsigned int mycss_property_shared_get_value_type(mycss_entry_t* entry, mycss_token_t* token, myhtml_string_t* str);
bool mycss_property_shared_by_value_type(mycss_entry_t* entry, mycss_token_t* token, unsigned int* value_type, unsigned int check_type, myhtml_string_t* str);
bool mycss_property_shared_width(mycss_entry_t* entry, mycss_token_t* token, void** value, unsigned int* value_type, myhtml_string_t* str);
bool mycss_property_shared_height(mycss_entry_t* entry, mycss_token_t* token, void** value, unsigned int* value_type, myhtml_string_t* str);
bool mycss_property_shared_line_width(mycss_entry_t* entry, mycss_token_t* token, void** value, unsigned int* value_type, myhtml_string_t* str);
bool mycss_property_shared_line_style(mycss_entry_t* entry, mycss_token_t* token, unsigned int* value_type, myhtml_string_t* str);

void mycss_property_shared_destroy_string(myhtml_string_t* str);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MyHTML_MyCSS_PROPERTY_SHARED_H */
