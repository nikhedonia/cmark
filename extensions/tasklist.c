#include "tasklist.h"
#include <parser.h>

static void postprocess_list(cmark_parser *parser, cmark_iter *iter);

static bool postprocess_item(cmark_parser *parser, cmark_iter *iter) {
  cmark_event_type ev;
  cmark_node *node;
  bool first = true,
       was_task_list_item = false;

  printf("  postprocess_item\n");

  while ((ev = cmark_iter_next(iter)) != CMARK_EVENT_DONE) {
    node = cmark_iter_get_node(iter);

    if (ev == CMARK_EVENT_ENTER && node->type != CMARK_NODE_PARAGRAPH && node->type != CMARK_NODE_TEXT)
      first = false;

    if (ev == CMARK_EVENT_ENTER && node->type == CMARK_NODE_LIST) {
      printf("item recurse list\n");
      postprocess_list(parser, iter);
      continue;
    }

    if (ev == CMARK_EVENT_ENTER && node->type == CMARK_NODE_ITEM) {
      printf("item recurse item\n");
      postprocess_item(parser, iter);
      continue;
    }

    if (ev == CMARK_EVENT_EXIT && node->type == CMARK_NODE_ITEM) {
      return was_task_list_item;
    }

    if (first && ev == CMARK_EVENT_ENTER && node->type == CMARK_NODE_TEXT) {
      printf("checking node with content {{{%s}}} for tasklist\n", node->as.literal.data);
      if (node->as.literal.len > 3 &&
          node->as.literal.data[0] == '[' &&
          node->as.literal.data[2] == ']' &&
          cmark_isspace(node->as.literal.data[3]) &&
          (cmark_isspace(node->as.literal.data[1]) || strchr("xX", node->as.literal.data[1]) != NULL)) {
        printf("hit\n");
        was_task_list_item = true;
        cmark_node_own(node);
        node->as.literal.len -= 3;
        memmove(node->as.literal.data, node->as.literal.data + 3, node->as.literal.len);
        cmark_node *n = cmark_node_new_with_mem(CMARK_NODE_HTML_INLINE, parser->mem);
        n->as.literal = cmark_chunk_literal(
            cmark_isspace(node->as.literal.data[1]) ?
              "<input type=\"checkbox\" class=\"task-list-item\" disabled>" :
              "<input type=\"checkbox\" class=\"task-list-item\" checked disabled>");
        cmark_node_insert_before(node, n);
      }
    }
  }

  return was_task_list_item;
}

static void postprocess_list(cmark_parser *parser, cmark_iter *iter) {
  cmark_event_type ev;
  cmark_node *node;
  printf("  postprocess_list\n");

  while ((ev = cmark_iter_next(iter)) != CMARK_EVENT_DONE) {
    node = cmark_iter_get_node(iter);

    if (ev == CMARK_EVENT_ENTER && node->type == CMARK_NODE_LIST) {
      printf("recurse list\n");
      postprocess_list(parser, iter);
      continue;
    }

    if (ev == CMARK_EVENT_ENTER && node->type == CMARK_NODE_ITEM) {
      printf("item\n");
      postprocess_item(parser, iter);
      continue;
    }

    if (ev == CMARK_EVENT_EXIT && node->type == CMARK_NODE_LIST) {
      return;
    }
  }
}

static cmark_node *postprocess(cmark_syntax_extension *ext, cmark_parser *parser, cmark_node *root) {
  cmark_iter *iter;
  cmark_event_type ev;
  cmark_node *node;

  cmark_consolidate_text_nodes(root);
  iter = cmark_iter_new(root);

  while ((ev = cmark_iter_next(iter)) != CMARK_EVENT_DONE) {
    node = cmark_iter_get_node(iter);

    if (ev == CMARK_EVENT_ENTER && node->type == CMARK_NODE_LIST) {
      printf("start list\n");
      postprocess_list(parser, iter);
      continue;
    }
  }

  cmark_iter_free(iter);

  return root;
}

cmark_syntax_extension *create_tasklist_extension(void) {
  cmark_syntax_extension *ext = cmark_syntax_extension_new("tasklist");
  cmark_syntax_extension_set_postprocess_func(ext, postprocess);
  return ext;
}
