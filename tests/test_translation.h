/*************************************************************************/
/*  test_translation.h                                                   */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           Fox ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Fox Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef TEST_TRANSLATION_H
#define TEST_TRANSLATION_H

#include "core/string/optimized_translation.h"
#include "core/string/translation.h"
#include "core/string/translation_po.h"

#include "thirdparty/doctest/doctest.h"

namespace TestTranslation {

TEST_CASE("[Translation] Messages") {
	Ref<Translation> translation = memnew(Translation);
	translation->set_locale("fr");
	translation->add_message("Hello", "Bonjour");
	CHECK(translation->get_message("Hello") == "Bonjour");

	translation->erase_message("Hello");
	// The message no longer exists, so it returns an empty string instead.
	CHECK(translation->get_message("Hello") == "");

	List<StringName> messages;
	translation->get_message_list(&messages);
	CHECK(translation->get_message_count() == 0);
	CHECK(messages.size() == 0);

	translation->add_message("Hello2", "Bonjour2");
	translation->add_message("Hello3", "Bonjour3");
	messages.clear();
	translation->get_message_list(&messages);
	CHECK(translation->get_message_count() == 2);
	CHECK(messages.size() == 2);
	// Messages are stored in a Map, don't assume ordering.
	CHECK(messages.find("Hello2"));
	CHECK(messages.find("Hello3"));
}

TEST_CASE("[TranslationPO] Messages with context") {
	Ref<TranslationPO> translation = memnew(TranslationPO);
	translation->set_locale("fr");
	translation->add_message("Hello", "Bonjour");
	translation->add_message("Hello", "Salut", "friendly");
	CHECK(translation->get_message("Hello") == "Bonjour");
	CHECK(translation->get_message("Hello", "friendly") == "Salut");
	CHECK(translation->get_message("Hello", "nonexistent_context") == "");

	// Only remove the message for the default context, not the "friendly" context.
	translation->erase_message("Hello");
	// The message no longer exists, so it returns an empty string instead.
	CHECK(translation->get_message("Hello") == "");
	CHECK(translation->get_message("Hello", "friendly") == "Salut");
	CHECK(translation->get_message("Hello", "nonexistent_context") == "");

	List<StringName> messages;
	translation->get_message_list(&messages);

	// `get_message_count()` takes all contexts into account.
	CHECK(translation->get_message_count() == 1);
	// Only the default context is taken into account.
	// Since "Hello" is now only present in a non-default context, it is not counted in the list of messages.
	CHECK(messages.size() == 0);

	translation->add_message("Hello2", "Bonjour2");
	translation->add_message("Hello2", "Salut2", "friendly");
	translation->add_message("Hello3", "Bonjour3");
	messages.clear();
	translation->get_message_list(&messages);

	// `get_message_count()` takes all contexts into account.
	CHECK(translation->get_message_count() == 4);
	// Only the default context is taken into account.
	CHECK(messages.size() == 2);
	// Messages are stored in a Map, don't assume ordering.
	CHECK(messages.find("Hello2"));
	CHECK(messages.find("Hello3"));
}

TEST_CASE("[TranslationPO] Plural messages") {
	Ref<TranslationPO> translation = memnew(TranslationPO);
	translation->set_locale("fr");
	translation->set_plural_rule("Plural-Forms: nplurals=2; plural=(n >= 2);");
	CHECK(translation->get_plural_forms() == 2);

	PackedStringArray plurals;
	plurals.push_back("Il y a %d pomme");
	plurals.push_back("Il y a %d pommes");
	translation->add_plural_message("There are %d apples", plurals);
	ERR_PRINT_OFF;
	// This is invalid, as the number passed to `get_plural_message()` may not be negative.
	CHECK(vformat(translation->get_plural_message("There are %d apples", "", -1), -1) == "");
	ERR_PRINT_ON;
	CHECK(vformat(translation->get_plural_message("There are %d apples", "", 0), 0) == "Il y a 0 pomme");
	CHECK(vformat(translation->get_plural_message("There are %d apples", "", 1), 1) == "Il y a 1 pomme");
	CHECK(vformat(translation->get_plural_message("There are %d apples", "", 2), 2) == "Il y a 2 pommes");
}

TEST_CASE("[OptimizedTranslation] Generate from Translation and read messages") {
	Ref<Translation> translation = memnew(Translation);
	translation->set_locale("fr");
	translation->add_message("Hello", "Bonjour");
	translation->add_message("Hello2", "Bonjour2");
	translation->add_message("Hello3", "Bonjour3");

	Ref<OptimizedTranslation> optimized_translation = memnew(OptimizedTranslation);
	optimized_translation->generate(translation);
	CHECK(optimized_translation->get_message("Hello") == "Bonjour");
	CHECK(optimized_translation->get_message("Hello2") == "Bonjour2");
	CHECK(optimized_translation->get_message("Hello3") == "Bonjour3");
	CHECK(optimized_translation->get_message("DoesNotExist") == "");

	List<StringName> messages;
	// `get_message_list()` can't return the list of messages stored in an OptimizedTranslation.
	optimized_translation->get_message_list(&messages);
	CHECK(optimized_translation->get_message_count() == 0);
	CHECK(messages.size() == 0);
}

} // namespace TestTranslation

#endif // TEST_TRANSLATION_H
