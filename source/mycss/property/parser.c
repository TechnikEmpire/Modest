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

#include "mycss/property/parser.h"

static bool mycss_property_parser_destroy_string(myhtml_string_t* str, bool return_value)
{
    mycss_property_shared_destroy_string(str);
    return return_value;
}

/////////////////////////////////////////////////////////
//// CSS Property
////
/////////////////////////////////////////////////////////
bool mycss_property_parser_undef(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

/* width height */
bool mycss_property_parser_width(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE)
        return true;
    
    myhtml_string_t str = {0};
    mycss_declaration_entry_t* declr_entry = entry->declaration->entry_last;
    
    if(mycss_property_shared_width(entry, token, &declr_entry->value, &declr_entry->value_type, &str)) {
        return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_find_important(entry));
    }
    
    return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_parse_error(entry));
}

bool mycss_property_parser_height(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE)
        return true;
    
    myhtml_string_t str = {0};
    mycss_declaration_entry_t* declr_entry = entry->declaration->entry_last;
    
    if(mycss_property_shared_height(entry, token, &declr_entry->value, &declr_entry->value_type, &str))
        return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_find_important(entry));
    
    return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_parse_error(entry));
}

/* padding */
bool mycss_property_parser_padding(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE)
        return true;
    
    mycss_declaration_entry_t* dec_entry = entry->declaration->entry_last;
    
    if(dec_entry->value == NULL)
        dec_entry->value = mycss_values_create(entry, sizeof(mycss_values_shorthand_four_t));
    
    mycss_values_shorthand_four_t *value = dec_entry->value;
    
    if(mycss_property_shared_check_declaration_end(entry, token)) {
        if(value->one == NULL) {
            dec_entry->value = mycss_values_destroy(entry, value);
            return mycss_property_shared_switch_to_parse_error(entry);
        }
        
        return true;
    }
    
    myhtml_string_t str = {0};
    
    if(value->one == NULL) {
        mycss_declaration_entry_t* one = mycss_declaration_entry_create(entry->declaration, NULL);
        
        if(mycss_property_shared_length(entry, token, &one->value, &one->value_type, &str) ||
           mycss_property_shared_default(entry, token, &one->value_type, &str))
        {
            one->type = MyCSS_PROPERTY_TYPE_PADDING_TOP;
            value->one = one;
            
            return mycss_property_parser_destroy_string(&str, true);
        }
        
        mycss_declaration_entry_destroy(entry->declaration, one);
        dec_entry->value = mycss_values_destroy(entry, value);
    }
    else if(value->two == NULL) {
        mycss_declaration_entry_t* two = mycss_declaration_entry_create(entry->declaration, NULL);
        
        if(mycss_property_shared_length(entry, token, &two->value, &two->value_type, &str) ||
           mycss_property_shared_default(entry, token, &two->value_type, &str))
        {
            two->type = MyCSS_PROPERTY_TYPE_PADDING_RIGHT;
            value->two = two;
            
            return mycss_property_parser_destroy_string(&str, true);
        }
        
        mycss_declaration_entry_destroy(entry->declaration, value->one);
        mycss_declaration_entry_destroy(entry->declaration, two);
        
        dec_entry->value = mycss_values_destroy(entry, value);
    }
    else if(value->three == NULL) {
        mycss_declaration_entry_t* three = mycss_declaration_entry_create(entry->declaration, NULL);
        
        if(mycss_property_shared_length(entry, token, &three->value, &three->value_type, &str) ||
           mycss_property_shared_default(entry, token, &three->value_type, &str))
        {
            three->type = MyCSS_PROPERTY_TYPE_PADDING_BOTTOM;
            value->three = three;
            
            return mycss_property_parser_destroy_string(&str, true);
        }
        
        mycss_declaration_entry_destroy(entry->declaration, value->one);
        mycss_declaration_entry_destroy(entry->declaration, value->two);
        mycss_declaration_entry_destroy(entry->declaration, three);
        
        dec_entry->value = mycss_values_destroy(entry, value);
    }
    else if(value->four == NULL) {
        mycss_declaration_entry_t* four = mycss_declaration_entry_create(entry->declaration, NULL);
        
        if(mycss_property_shared_length(entry, token, &four->value, &four->value_type, &str) ||
           mycss_property_shared_default(entry, token, &four->value_type, &str))
        {
            four->type = MyCSS_PROPERTY_TYPE_PADDING_LEFT;
            value->four = four;
            
            return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_find_important(entry));
        }
        
        mycss_declaration_entry_destroy(entry->declaration, value->one);
        mycss_declaration_entry_destroy(entry->declaration, value->two);
        mycss_declaration_entry_destroy(entry->declaration, value->three);
        mycss_declaration_entry_destroy(entry->declaration, four);
        
        dec_entry->value = mycss_values_destroy(entry, value);
    }
    
    return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_parse_error(entry));
}

bool mycss_property_parser_padding_X(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE)
        return true;
    
    myhtml_string_t str = {0};
    mycss_declaration_entry_t* dec_entry = entry->declaration->entry_last;
    
    if(mycss_property_shared_length(entry, token, &dec_entry->value, &dec_entry->value_type, &str) ||
       mycss_property_shared_default(entry, token, &dec_entry->value_type, &str))
    {
        return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_find_important(entry));
    }
    
    return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_parse_error(entry));
}

bool mycss_property_parser_padding_bottom(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_parser_padding_X(entry, token, last_response);
}

bool mycss_property_parser_padding_left(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_parser_padding_X(entry, token, last_response);
}

bool mycss_property_parser_padding_right(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_parser_padding_X(entry, token, last_response);
}

bool mycss_property_parser_padding_top(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_parser_padding_X(entry, token, last_response);
}

/* margin */
bool mycss_property_parser_margin_X(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE)
        return true;
    
    myhtml_string_t str = {0};
    mycss_declaration_entry_t* dec_entry = entry->declaration->entry_last;
    
    if(mycss_property_shared_length(entry, token, &dec_entry->value, &dec_entry->value_type, &str) ||
       mycss_property_shared_default(entry, token, &dec_entry->value_type, &str) ||
       mycss_property_shared_by_value_type(entry, token, &dec_entry->value_type, MyCSS_PROPERTY_MARGIN_AUTO, &str))
    {
        return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_find_important(entry));
    }
    
    return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_parse_error(entry));
}

bool mycss_property_parser_margin(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE)
        return true;
    
    mycss_declaration_entry_t* dec_entry = entry->declaration->entry_last;
    
    if(dec_entry->value == NULL)
        dec_entry->value = mycss_values_create(entry, sizeof(mycss_values_shorthand_four_t));
    
    mycss_values_shorthand_four_t *value = dec_entry->value;
    
    if(mycss_property_shared_check_declaration_end(entry, token)) {
        if(value->one == NULL) {
            dec_entry->value = mycss_values_destroy(entry, value);
            return mycss_property_shared_switch_to_parse_error(entry);
        }
        
        return true;
    }
    
    myhtml_string_t str = {0};
    
    if(value->one == NULL) {
        mycss_declaration_entry_t* one = mycss_declaration_entry_create(entry->declaration, NULL);
        
        if(mycss_property_shared_length(entry, token, &one->value, &one->value_type, &str) ||
           mycss_property_shared_default(entry, token, &one->value_type, &str) ||
           mycss_property_shared_by_value_type(entry, token, &one->value_type, MyCSS_PROPERTY_MARGIN_AUTO, &str))
        {
            one->type = MyCSS_PROPERTY_TYPE_MARGIN_TOP;
            value->one = one;
            
            return mycss_property_parser_destroy_string(&str, true);
        }
        
        mycss_declaration_entry_destroy(entry->declaration, one);
        dec_entry->value = mycss_values_destroy(entry, value);
    }
    else if(value->two == NULL) {
        mycss_declaration_entry_t* two = mycss_declaration_entry_create(entry->declaration, NULL);
        
        if(mycss_property_shared_length(entry, token, &two->value, &two->value_type, &str) ||
           mycss_property_shared_default(entry, token, &two->value_type, &str) ||
           mycss_property_shared_by_value_type(entry, token, &two->value_type, MyCSS_PROPERTY_MARGIN_AUTO, &str))
        {
            two->type = MyCSS_PROPERTY_TYPE_MARGIN_RIGHT;
            value->two = two;
            
            return mycss_property_parser_destroy_string(&str, true);
        }
        
        mycss_declaration_entry_destroy(entry->declaration, value->one);
        mycss_declaration_entry_destroy(entry->declaration, two);
        dec_entry->value = mycss_values_destroy(entry, value);
    }
    else if(value->three == NULL) {
        mycss_declaration_entry_t* three = mycss_declaration_entry_create(entry->declaration, NULL);
        
        if(mycss_property_shared_length(entry, token, &three->value, &three->value_type, &str) ||
           mycss_property_shared_default(entry, token, &three->value_type, &str) ||
           mycss_property_shared_by_value_type(entry, token, &three->value_type, MyCSS_PROPERTY_MARGIN_AUTO, &str))
        {
            three->type = MyCSS_PROPERTY_TYPE_MARGIN_BOTTOM;
            value->three = three;
            
            return mycss_property_parser_destroy_string(&str, true);
        }
        
        mycss_declaration_entry_destroy(entry->declaration, value->one);
        mycss_declaration_entry_destroy(entry->declaration, value->two);
        mycss_declaration_entry_destroy(entry->declaration, three);
        dec_entry->value = mycss_values_destroy(entry, value);
    }
    else if(value->four == NULL) {
        mycss_declaration_entry_t* four = mycss_declaration_entry_create(entry->declaration, NULL);
        
        if(mycss_property_shared_length(entry, token, &four->value, &four->value_type, &str) ||
           mycss_property_shared_default(entry, token, &four->value_type, &str) ||
           mycss_property_shared_by_value_type(entry, token, &four->value_type, MyCSS_PROPERTY_MARGIN_AUTO, &str))
        {
            four->type = MyCSS_PROPERTY_TYPE_MARGIN_LEFT;
            value->four = four;
            
            return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_find_important(entry));
        }
        
        mycss_declaration_entry_destroy(entry->declaration, value->one);
        mycss_declaration_entry_destroy(entry->declaration, value->two);
        mycss_declaration_entry_destroy(entry->declaration, value->three);
        mycss_declaration_entry_destroy(entry->declaration, four);
        dec_entry->value = mycss_values_destroy(entry, value);
    }
    
    return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_parse_error(entry));
}

bool mycss_property_parser_margin_bottom(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_parser_margin_X(entry, token, last_response);
}

bool mycss_property_parser_margin_left(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_parser_margin_X(entry, token, last_response);
}

bool mycss_property_parser_margin_right(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_parser_margin_X(entry, token, last_response);
}

bool mycss_property_parser_margin_top(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_parser_margin_X(entry, token, last_response);
}

/* display */
bool mycss_property_parser_display(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE)
        return true;
    
    myhtml_string_t str = {0};
    
    if(token->type == MyCSS_TOKEN_TYPE_IDENT)
    {
        mycss_declaration_entry_t* dec_entry = entry->declaration->entry_last;
        mycss_token_data_to_string(entry, token, &str, true, false);
        
        dec_entry->value_type = mycss_property_value_type_by_name(str.data, str.length);
        
        switch (dec_entry->value_type) {
            case MyCSS_PROPERTY_DISPLAY_BLOCK:
            case MyCSS_PROPERTY_DISPLAY_CONTENTS:
            case MyCSS_PROPERTY_DISPLAY_FLEX:
            case MyCSS_PROPERTY_DISPLAY_FLOW:
            case MyCSS_PROPERTY_DISPLAY_FLOW_ROOT:
            case MyCSS_PROPERTY_DISPLAY_GRID:
            case MyCSS_PROPERTY_DISPLAY_INLINE:
            case MyCSS_PROPERTY_DISPLAY_INLINE_BLOCK:
            case MyCSS_PROPERTY_DISPLAY_INLINE_FLEX:
            case MyCSS_PROPERTY_DISPLAY_INLINE_GRID:
            case MyCSS_PROPERTY_DISPLAY_INLINE_LIST_ITEM:
            case MyCSS_PROPERTY_DISPLAY_INLINE_TABLE:
            case MyCSS_PROPERTY_DISPLAY_LIST_ITEM:
            case MyCSS_PROPERTY_DISPLAY_NONE:
            case MyCSS_PROPERTY_DISPLAY_RUBY:
            case MyCSS_PROPERTY_DISPLAY_RUBY_BASE:
            case MyCSS_PROPERTY_DISPLAY_RUBY_BASE_CONTAINER:
            case MyCSS_PROPERTY_DISPLAY_RUBY_TEXT:
            case MyCSS_PROPERTY_DISPLAY_RUBY_TEXT_CONTAINER:
            case MyCSS_PROPERTY_DISPLAY_RUN_IN:
            case MyCSS_PROPERTY_DISPLAY_TABLE:
            case MyCSS_PROPERTY_DISPLAY_TABLE_CAPTION:
            case MyCSS_PROPERTY_DISPLAY_TABLE_CELL:
            case MyCSS_PROPERTY_DISPLAY_TABLE_COLUMN:
            case MyCSS_PROPERTY_DISPLAY_TABLE_COLUMN_GROUP:
            case MyCSS_PROPERTY_DISPLAY_TABLE_FOOTER_GROUP:
            case MyCSS_PROPERTY_DISPLAY_TABLE_HEADER_GROUP:
            case MyCSS_PROPERTY_DISPLAY_TABLE_ROW:
            case MyCSS_PROPERTY_DISPLAY_TABLE_ROW_GROUP:
                break;
                
            default:
                if(mycss_property_shared_default(entry, token, &dec_entry->value_type, &str) == false) {
                    dec_entry->value_type = MyCSS_PROPERTY_TYPE_UNDEF;
                    return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_parse_error(entry));
                }
                break;
        }
    }
    
    return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_find_important(entry));
}

/* border width */
bool mycss_property_parser_border_width(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE)
        return true;
    
    mycss_declaration_entry_t* dec_entry = entry->declaration->entry_last;
    
    if(dec_entry->value == NULL)
        dec_entry->value = mycss_values_create(entry, sizeof(mycss_values_shorthand_four_t));
    
    mycss_values_shorthand_four_t *value = dec_entry->value;
    
    if(mycss_property_shared_check_declaration_end(entry, token)) {
        if(value->one == NULL) {
            dec_entry->value = mycss_values_destroy(entry, value);
            return mycss_property_shared_switch_to_parse_error(entry);
        }
        
        return true;
    }
    
    myhtml_string_t str = {0};
    
    if(value->one == NULL) {
        mycss_declaration_entry_t* in_declr_entry = mycss_declaration_entry_create(entry->declaration, NULL);
        
        if(mycss_property_shared_line_width(entry, token, &in_declr_entry->value, &in_declr_entry->value_type, &str))
        {
            in_declr_entry->type = MyCSS_PROPERTY_TYPE_BORDER_TOP_WIDTH;
            value->one = in_declr_entry;
            
            return mycss_property_parser_destroy_string(&str, true);
        }
        
        mycss_declaration_entry_destroy(entry->declaration, in_declr_entry);
        dec_entry->value = mycss_values_destroy(entry, value);
    }
    else if(value->two == NULL) {
        mycss_declaration_entry_t* in_declr_entry = mycss_declaration_entry_create(entry->declaration, NULL);
        
        if(mycss_property_shared_line_width(entry, token, &in_declr_entry->value, &in_declr_entry->value_type, &str))
        {
            in_declr_entry->type = MyCSS_PROPERTY_TYPE_BORDER_RIGHT_WIDTH;
            value->two = in_declr_entry;
            
            return mycss_property_parser_destroy_string(&str, true);
        }
        
        mycss_declaration_entry_destroy(entry->declaration, value->one);
        mycss_declaration_entry_destroy(entry->declaration, in_declr_entry);
        dec_entry->value = mycss_values_destroy(entry, value);
    }
    else if(value->three == NULL) {
        mycss_declaration_entry_t* in_declr_entry = mycss_declaration_entry_create(entry->declaration, NULL);
        
        if(mycss_property_shared_line_width(entry, token, &in_declr_entry->value, &in_declr_entry->value_type, &str))
        {
            in_declr_entry->type = MyCSS_PROPERTY_TYPE_BORDER_BOTTOM_WIDTH;
            value->three = in_declr_entry;
            
            return mycss_property_parser_destroy_string(&str, true);
        }
        
        mycss_declaration_entry_destroy(entry->declaration, value->one);
        mycss_declaration_entry_destroy(entry->declaration, value->two);
        mycss_declaration_entry_destroy(entry->declaration, in_declr_entry);
        dec_entry->value = mycss_values_destroy(entry, value);
    }
    else if(value->four == NULL) {
        mycss_declaration_entry_t* in_declr_entry = mycss_declaration_entry_create(entry->declaration, NULL);
        
        if(mycss_property_shared_line_width(entry, token, &in_declr_entry->value, &in_declr_entry->value_type, &str))
        {
            in_declr_entry->type = MyCSS_PROPERTY_TYPE_BORDER_LEFT_WIDTH;
            value->four = in_declr_entry;
            
            return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_find_important(entry));
        }
        
        mycss_declaration_entry_destroy(entry->declaration, value->one);
        mycss_declaration_entry_destroy(entry->declaration, value->two);
        mycss_declaration_entry_destroy(entry->declaration, value->three);
        mycss_declaration_entry_destroy(entry->declaration, in_declr_entry);
        dec_entry->value = mycss_values_destroy(entry, value);
    }
    
    return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_parse_error(entry));;
}

bool mycss_property_parser_border_top_width(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE)
        return true;
    
    myhtml_string_t str = {0};
    mycss_declaration_entry_t* declr_entry = entry->declaration->entry_last;
    
    if(mycss_property_shared_line_width(entry, token, &declr_entry->value, &declr_entry->value_type, &str)) {
        return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_find_important(entry));
    }
    
    return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_parse_error(entry));
}

bool mycss_property_parser_border_right_width(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_parser_border_top_width(entry, token, last_response);
}

bool mycss_property_parser_border_bottom_width(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_parser_border_top_width(entry, token, last_response);
}

bool mycss_property_parser_border_left_width(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_parser_border_top_width(entry, token, last_response);
}

/* border style */
bool mycss_property_parser_border_style(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE)
        return true;
    
    mycss_declaration_entry_t* dec_entry = entry->declaration->entry_last;
    
    if(dec_entry->value == NULL)
        dec_entry->value = mycss_values_create(entry, sizeof(mycss_values_shorthand_four_t));
    
    mycss_values_shorthand_four_t *value = dec_entry->value;
    
    if(mycss_property_shared_check_declaration_end(entry, token)) {
        if(value->one == NULL) {
            dec_entry->value = mycss_values_destroy(entry, value);
            return mycss_property_shared_switch_to_parse_error(entry);
        }
        
        return true;
    }
    
    myhtml_string_t str = {0};
    
    if(value->one == NULL) {
        mycss_declaration_entry_t* in_declr_entry = mycss_declaration_entry_create(entry->declaration, NULL);
        
        if(mycss_property_shared_line_style(entry, token, &in_declr_entry->value_type, &str) == false)
        {
            in_declr_entry->type = MyCSS_PROPERTY_TYPE_BORDER_TOP_STYLE;
            value->one = in_declr_entry;
            
            return mycss_property_parser_destroy_string(&str, true);
        }
        
        mycss_declaration_entry_destroy(entry->declaration, in_declr_entry);
        dec_entry->value = mycss_values_destroy(entry, value);
    }
    else if(value->two == NULL) {
        mycss_declaration_entry_t* in_declr_entry = mycss_declaration_entry_create(entry->declaration, NULL);
        
        if(mycss_property_shared_line_style(entry, token, &in_declr_entry->value_type, &str) == false)
        {
            in_declr_entry->type = MyCSS_PROPERTY_TYPE_BORDER_RIGHT_STYLE;
            value->two = in_declr_entry;
            
            return mycss_property_parser_destroy_string(&str, true);
        }
        
        mycss_declaration_entry_destroy(entry->declaration, value->one);
        mycss_declaration_entry_destroy(entry->declaration, in_declr_entry);
        dec_entry->value = mycss_values_destroy(entry, value);
    }
    else if(value->three == NULL) {
        mycss_declaration_entry_t* in_declr_entry = mycss_declaration_entry_create(entry->declaration, NULL);
        
        if(mycss_property_shared_line_style(entry, token, &in_declr_entry->value_type, &str) == false)
        {
            in_declr_entry->type = MyCSS_PROPERTY_TYPE_BORDER_BOTTOM_STYLE;
            value->three = in_declr_entry;
            
            return mycss_property_parser_destroy_string(&str, true);
        }
        
        mycss_declaration_entry_destroy(entry->declaration, value->one);
        mycss_declaration_entry_destroy(entry->declaration, value->two);
        mycss_declaration_entry_destroy(entry->declaration, in_declr_entry);
        dec_entry->value = mycss_values_destroy(entry, value);
    }
    else if(value->four == NULL) {
        mycss_declaration_entry_t* in_declr_entry = mycss_declaration_entry_create(entry->declaration, NULL);
        
        if(mycss_property_shared_line_style(entry, token, &in_declr_entry->value_type, &str) == false)
        {
            in_declr_entry->type = MyCSS_PROPERTY_TYPE_BORDER_LEFT_STYLE;
            value->four = in_declr_entry;
            
            return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_find_important(entry));
        }
        
        mycss_declaration_entry_destroy(entry->declaration, value->one);
        mycss_declaration_entry_destroy(entry->declaration, value->two);
        mycss_declaration_entry_destroy(entry->declaration, value->three);
        mycss_declaration_entry_destroy(entry->declaration, in_declr_entry);
        dec_entry->value = mycss_values_destroy(entry, value);
    }
    
    return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_parse_error(entry));
}

bool mycss_property_parser_border_top_style(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE)
        return true;
    
    myhtml_string_t str = {0};
    mycss_declaration_entry_t* declr_entry = entry->declaration->entry_last;
    
    if(mycss_property_shared_line_style(entry, token, &declr_entry->value_type, &str)) {
        return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_find_important(entry));
    }
    
    return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_parse_error(entry));
}

bool mycss_property_parser_border_right_style(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_parser_border_top_style(entry, token, last_response);
}

bool mycss_property_parser_border_bottom_style(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_parser_border_top_style(entry, token, last_response);
}

bool mycss_property_parser_border_left_style(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_parser_border_top_style(entry, token, last_response);
}

/* border radius */
bool mycss_property_parser_border_radius(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_border_top_right_radius(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE)
        return true;
    
    mycss_declaration_entry_t* dec_entry = entry->declaration->entry_last;
    
    if(dec_entry->value == NULL)
        dec_entry->value = mycss_values_create(entry, sizeof(mycss_values_shorthand_two_type_t));
    
    mycss_values_shorthand_two_type_t *short_two_type = dec_entry->value;
    
    if(mycss_property_shared_check_declaration_end(entry, token)) {
        if(short_two_type->one == NULL) {
            dec_entry->value = mycss_values_destroy(entry, short_two_type);
            return mycss_property_shared_switch_to_parse_error(entry);
        }
        
        return true;
    }
    
    myhtml_string_t str = {0};
    
    if(short_two_type->one == NULL) {
        if(mycss_property_shared_length_percentage(entry, token, &short_two_type->one, &short_two_type->type_one, &str) ||
           mycss_property_shared_default(entry, token, &short_two_type->type_one, &str))
        {
            return mycss_property_parser_destroy_string(&str, true);
        }
        
        dec_entry->value = mycss_values_destroy(entry, short_two_type);
    }
    else if(short_two_type->two == NULL) {
        if(mycss_property_shared_length_percentage(entry, token, &short_two_type->two, &short_two_type->type_two, &str) ||
           mycss_property_shared_default(entry, token, &short_two_type->type_two, &str))
        {
            return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_find_important(entry));
        }
        
        short_two_type->one = mycss_values_destroy(entry, short_two_type->one);
        dec_entry->value = mycss_values_destroy(entry, short_two_type);
    }
    
    return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_parse_error(entry));
}

bool mycss_property_parser_border_top_left_radius(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_parser_border_top_right_radius(entry, token, last_response);
}

bool mycss_property_parser_border_bottom_right_radius(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_parser_border_top_right_radius(entry, token, last_response);
}

bool mycss_property_parser_border_bottom_left_radius(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_parser_border_top_right_radius(entry, token, last_response);
}

/* box sizing */
bool mycss_property_parser_box_sizing(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE)
        return true;
    
    if(token->type != MyCSS_TOKEN_TYPE_IDENT)
        return mycss_property_shared_switch_to_parse_error(entry);
    
    myhtml_string_t str = {0};
    mycss_token_data_to_string(entry, token, &str, true, false);
    
    mycss_declaration_entry_t* dec_entry = entry->declaration->entry_last;
    dec_entry->value_type = mycss_property_value_type_by_name(str.data, str.length);
    
    switch (dec_entry->value_type) {
        case MyCSS_PROPERTY_BOX_SIZING_CONTENT_BOX:
        case MyCSS_PROPERTY_BOX_SIZING_BORDER_BOX:
            /* default values */
        case MyCSS_PROPERTY_VALUE_INHERIT:
        case MyCSS_PROPERTY_VALUE_INITIAL:
        case MyCSS_PROPERTY_VALUE_UNSET:
            return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_find_important(entry));
            
        default:
            break;
    }
    
    return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_parse_error(entry));
}

/* vertical align */
bool mycss_property_parser_vertical_align(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE)
        return true;
    
    myhtml_string_t str = {0};
    mycss_declaration_entry_t* dec_entry = entry->declaration->entry_last;
    
    if(mycss_property_shared_length_percentage(entry, token, &dec_entry->value, &dec_entry->value_type, &str))
        return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_find_important(entry));
    
    if(token->type != MyCSS_TOKEN_TYPE_IDENT)
        return mycss_property_shared_switch_to_parse_error(entry);
    
    if(str.data == NULL)
        mycss_token_data_to_string(entry, token, &str, true, false);
    
    dec_entry->value_type = mycss_property_value_type_by_name(str.data, str.length);
    
    switch (dec_entry->value_type) {
        case MyCSS_PROPERTY_VERTICAL_ALIGN_BASELINE:
        case MyCSS_PROPERTY_VERTICAL_ALIGN_SUB:
        case MyCSS_PROPERTY_VERTICAL_ALIGN_SUPER:
        case MyCSS_PROPERTY_VERTICAL_ALIGN_TOP:
        case MyCSS_PROPERTY_VERTICAL_ALIGN_TEXT_TOP:
        case MyCSS_PROPERTY_VERTICAL_ALIGN_MIDDLE:
        case MyCSS_PROPERTY_VERTICAL_ALIGN_BOTTOM:
        case MyCSS_PROPERTY_VERTICAL_ALIGN_TEXT_BOTTOM:
            /* default values */
        case MyCSS_PROPERTY_VALUE_INHERIT:
        case MyCSS_PROPERTY_VALUE_INITIAL:
        case MyCSS_PROPERTY_VALUE_UNSET:
            return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_find_important(entry));
            
        default:
            break;
    }
    
    return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_parse_error(entry));
}

/* line height */
bool mycss_property_parser_line_height(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE)
        return true;
    
    myhtml_string_t str = {0};
    mycss_declaration_entry_t* dec_entry = entry->declaration->entry_last;
    
    if(mycss_property_shared_length_percentage(entry, token, &dec_entry->value, &dec_entry->value_type, &str) ||
       mycss_property_shared_number(entry, token, &dec_entry->value, &dec_entry->value_type, &str))
    {
        return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_find_important(entry));
    }
    
    if(token->type != MyCSS_TOKEN_TYPE_IDENT)
        return mycss_property_shared_switch_to_parse_error(entry);
    
    if(str.data == NULL)
        mycss_token_data_to_string(entry, token, &str, true, false);
    
    dec_entry->value_type = mycss_property_value_type_by_name(str.data, str.length);
    
    switch (dec_entry->value_type) {
        case MyCSS_PROPERTY_LINE_HEIGHT_NORMAL:
            /* default values */
        case MyCSS_PROPERTY_VALUE_INHERIT:
        case MyCSS_PROPERTY_VALUE_INITIAL:
        case MyCSS_PROPERTY_VALUE_UNSET:
            return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_find_important(entry));
            
        default:
            dec_entry->value_type = MyCSS_PROPERTY_VALUE_UNDEF;
            break;
    }
    
    return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_parse_error(entry));
}

/* text decoration */
bool mycss_property_parser_text_decoration_line(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE)
        return true;
    
    if(token->type != MyCSS_TOKEN_TYPE_IDENT)
        return mycss_property_shared_switch_to_parse_error(entry);
    
    myhtml_string_t str = {0};
    mycss_declaration_entry_t* dec_entry = entry->declaration->entry_last;
    
    if(str.data == NULL)
        mycss_token_data_to_string(entry, token, &str, true, false);
    
    mycss_property_value_t valye_type = mycss_property_value_type_by_name(str.data, str.length);
    
    switch (dec_entry->value_type) {
        case MyCSS_PROPERTY_TEXT_DECORATION_LINE_UNDERLINE:
        case MyCSS_PROPERTY_TEXT_DECORATION_LINE_OVERLINE:
        case MyCSS_PROPERTY_TEXT_DECORATION_LINE_LINE_THROUGH:
        case MyCSS_PROPERTY_TEXT_DECORATION_LINE_BLINK:
            dec_entry->value_type |= valye_type;
            return mycss_property_parser_destroy_string(&str, true);
            
        case MyCSS_PROPERTY_TEXT_DECORATION_LINE_NONE:
            /* default values */
        case MyCSS_PROPERTY_VALUE_INHERIT:
        case MyCSS_PROPERTY_VALUE_INITIAL:
        case MyCSS_PROPERTY_VALUE_UNSET:
            dec_entry->value_type = valye_type;
            return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_find_important(entry));
            
        default:
            dec_entry->value_type = MyCSS_PROPERTY_VALUE_UNDEF;
            break;
    }
    
    return mycss_property_parser_destroy_string(&str, mycss_property_shared_switch_to_parse_error(entry));
}

/* not yet */
bool mycss_property_parser_align_content(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_align_items(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_align_self(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_animation(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_animation_delay(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_animation_direction(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_animation_duration(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_animation_fill_mode(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_animation_iteration_count(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_animation_name(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_animation_play_state(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_animation_timing_function(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_appearance(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_backface_visibility(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_background(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_background_attachment(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_background_clip(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_background_color(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_background_image(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_background_origin(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_background_position(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_background_repeat(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_background_size(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_bookmark_label(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_bookmark_level(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_bookmark_state(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_border(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_border_bottom(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_border_bottom_color(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_border_collapse(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_border_color(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_border_image(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_border_image_outset(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_border_image_repeat(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_border_image_slice(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_border_image_width(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_border_left(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_border_left_color(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_border_right(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_border_right_color(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_border_spacing(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_border_top(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_border_top_color(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_bottom(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_box_decoration_break(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_box_shadow(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_box_suppress(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_break_after(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_break_before(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_break_inside(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_caption_side(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_caret(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_caret_color(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_caret_shape(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_clear(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_clear_after(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_color(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_color_adjust(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_color_interpolation(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_color_interpolation_filters(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_color_rendering(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_column_count(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_column_fill(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_column_gap(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_column_rule(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_column_rule_color(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_column_rule_style(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_column_rule_width(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_column_span(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_column_width(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_columns(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_contain(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_content(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_continue(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_counter_increment(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_cue(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_cue_after(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_cue_before(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_cursor(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_direction(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_empty_cells(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_fill(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_fill_opacity(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_fill_rule(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_flex(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_flex_basis(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_flex_direction(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_flex_flow(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_flex_grow(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_flex_shrink(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_flex_wrap(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_float(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_float_displace(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_font(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_font_family(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_font_feature_settings(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_font_kerning(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_font_language_override(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_font_size(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_font_size_adjust(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_font_stretch(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_font_style(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_font_synthesis(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_font_variant(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_font_variant_alternates(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_font_variant_caps(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_font_variant_east_asian(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_font_variant_ligatures(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_font_variant_numeric(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_font_variant_position(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_font_weight(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_glyph_orientation_vertical(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_hanging_punctuation(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_hyphens(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_image_rendering(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_indent_edge_reset(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_inline_size_step(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_justify_content(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_left(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_letter_spacing(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_line_break(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_line_height_step(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_list_style(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_list_style_image(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_list_style_position(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_list_style_type(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_marker(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_marker_end(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_marker_mid(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_marker_side(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_marker_start(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_max_height(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_max_lines(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_max_width(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_min_height(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_min_width(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_nav_down(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_nav_left(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_nav_right(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_nav_up(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_offset_after(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_offset_before(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_offset_end(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_offset_start(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_opacity(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_order(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_orphans(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_outline(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_outline_color(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_outline_offset(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_outline_style(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_outline_width(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_overflow(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_overflow_wrap(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_overflow_x(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_overflow_y(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_pause(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_pause_after(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_pause_before(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_perspective(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_perspective_origin(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_position(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_presentation_level(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_quotes(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_region_fragment(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_resize(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_rest(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_rest_after(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_rest_before(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_right(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_ruby_align(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_ruby_merge(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_ruby_position(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_scroll_padding(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_scroll_snap_align(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_scroll_snap_margin(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_scroll_snap_stop(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_scroll_snap_type(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_shape_image_threshold(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_shape_margin(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_shape_outside(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_shape_rendering(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_speak(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_speak_as(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_string_set(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_stroke(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_stroke_dasharray(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_stroke_dashoffset(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_stroke_linecap(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_stroke_linejoin(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_stroke_miterlimit(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_stroke_opacity(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_stroke_width(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_tab_size(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_table_layout(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_align(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_align_all(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_align_last(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_combine_upright(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_decoration(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_decoration_color(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_decoration_skip(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_decoration_style(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_emphasis(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_emphasis_color(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_emphasis_position(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_emphasis_style(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_indent(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_justify(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_orientation(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_overflow(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_rendering(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_shadow(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_size_adjust(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_transform(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_text_underline_position(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_top(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_touch_action(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_transform(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_transform_box(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_transform_origin(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_transform_style(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_transition(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_transition_delay(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_transition_duration(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_transition_property(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_transition_timing_function(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_unicode_bidi(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_user_select(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_visibility(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_voice_balance(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_voice_duration(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_voice_family(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_voice_pitch(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_voice_range(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_voice_rate(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_voice_stress(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_voice_volume(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_white_space(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_widows(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_will_change(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_word_break(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_word_spacing(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_word_wrap(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_wrap_flow(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_wrap_through(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_writing_mode(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}

bool mycss_property_parser_z_index(mycss_entry_t* entry, mycss_token_t* token, bool last_response)
{
    return mycss_property_shared_switch_to_parse_error(entry);
}


