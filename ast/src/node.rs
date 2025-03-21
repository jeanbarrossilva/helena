// Copyright © 2025 Jean Silva
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
//                            http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under
// the License.

use std::fmt::{Display, Formatter};

use regex::Regex;

use crate::common::newline::NEWLINE;

/// Error that occurs when the name of a node that is required to follow a specific pattern does not
/// match it.
#[derive(Debug)]
pub struct UnmatchedPatternError {
  /// The message to be displayed that describes the error.
  message: String
}

impl Display for UnmatchedPatternError {
  fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
    Display::fmt(&self.message, f)
  }
}

impl std::error::Error for UnmatchedPatternError {}

/// A node (token) in the AST.
#[derive(Clone, Debug, Default, PartialEq)]
pub struct Node<'a> {
  /// Semantic name of this node.
  name: &'a str,

  /// Textual representation of this node in the AST.
  text: &'a str,

  /// Vertical position of this node in the AST. Corresponds to the number of the line in which it
  /// is located in the source file.
  column: u32,

  /// Horizontal position of this node in the AST. Corresponds to the index of the character that
  /// delimits the start of this node in the line of the source file in which it is located.
  row: u32,

  /// Expected next nodes; a [None] being present in this [Vec] means that this can be a leaf node,
  /// while [Some]s hold the nodes by which this one can be followed. Nothing prevents both
  /// scenarios from being true: there might be the expectation for this node to be a leaf node,
  /// but, in case it is not, for there to be other specific nodes that succeed it.
  ///
  /// ```rust
  /// use ast::node::Node;
  ///
  /// // Here, "a" can either be a leaf node — be the final one of the rule — or be followed by "b".
  /// Node::of("a", 0, 0).branch_to("b", |node| Ok(node)).and_then(|node| node.leaf()).unwrap();
  /// ```
  children: Vec<Option<Node<'a>>>
}

impl<'a> Node<'a> {
  /// Instantiates a node whose name is the given textual representation.
  pub fn of(text: &'a str, column: u32, row: u32) -> Self {
    Node::new(text, text, column, row)
  }

  /// Instantiates a node.
  pub(crate) fn new(name: &'a str, text: &'a str, column: u32, row: u32) -> Self {
    Node {
      name,
      text,
      column,
      row,
      children: Vec::new()
    }
  }
}

impl<'a> Node<'a> {
  /// Denotes that a node with the given textual representation can be the next one.
  pub fn branch_to<F: Fn(Self) -> Result<Self, UnmatchedPatternError>>(
    self,
    text: &'a str,
    chain: F
  ) -> Result<Self, UnmatchedPatternError> {
    self.branch_to_named(text, text, chain)
  }

  /// Denotes that this node can be the last one — a leaf — of the lexeme.
  pub fn leaf(mut self) -> Result<Self, UnmatchedPatternError> {
    if self.children.contains(&None) {
      return Ok(self);
    }
    self.children.push(None);
    Ok(self)
  }

  /// Denotes that a node whose text matches the given name, pattern and textual representation can
  /// be the next one.
  pub(crate) fn branch_to_named_and_patterned<
    E: Fn() -> String,
    C: Fn(Self) -> Result<Self, UnmatchedPatternError>
  >(
    self,
    name: &'a str,
    pattern: Regex,
    error_message: E,
    text: &'a str,
    chain: C
  ) -> Result<Self, UnmatchedPatternError> {
    if !pattern.is_match(text) {
      return Err(UnmatchedPatternError {
        message: error_message()
      });
    }
    self.branch_to_named(name, text, chain)
  }

  /// Denotes that a node with the given name and textual representation can be the next one.
  pub(crate) fn branch_to_named<F: Fn(Self) -> Result<Self, UnmatchedPatternError>>(
    mut self,
    name: &'a str,
    text: &'a str,
    chain: F
  ) -> Result<Self, UnmatchedPatternError> {
    let next_node = Self::new(name, text, self.next_column(text), self.next_row(text));
    let next_self = chain(next_node);
    if next_self.is_err() {
      return next_self;
    }
    self.children.push(next_self.ok());
    Ok(self)
  }

  /// Obtains the column in which the node that follows this one is expected to be.
  fn next_column(&'a self, text: &'a str) -> u32 {
    if self.column > 0 && self.next_row(text) == 0 {
      return self.column + 1;
    }
    self.column
  }

  /// Obtains the row in which the node that follows this one is expected to be.
  fn next_row(&'a self, text: &'a str) -> u32 {
    (self.row as usize + text.len()).min(100) as u32
  }

  /// Function called recursively by `fmt(&self, &mut Formatter<'_>)` for displaying this node
  /// with its children, formatting them according to their level — the amount of nodes from them to
  /// the root — in the AST.
  fn fmt(&'a self, f: &mut Formatter<'_>, level: &'a mut usize) -> std::fmt::Result {
    let mut ascii_tree = String::from("├─ ") + &*self.display_name() + NEWLINE;
    if *level > 0 {
      ascii_tree.insert_str(0, &*" ".repeat(*level * 3));
    }
    match write!(f, "{}", ascii_tree) {
      Ok(_) => (),
      Err(error) => return Err(error)
    };
    for child in &self.children {
      if child.is_none() {
        return write!(f, "├─");
      }
      let child_ascii_tree = child.clone().unwrap().fmt(f, &mut (*level + 1));
      if child_ascii_tree.is_err() {
        return child_ascii_tree;
      }
    }
    Ok(())
  }

  /// Obtains a name for this node based on its own that is suited for being displayed.
  fn display_name(&'a self) -> String {
    if self.name == self.text || self.text.chars().all(|char| char.is_ascii_whitespace()) {
      String::from(self.name)
    } else {
      format!("{} \"{}\"", self.name, self.text)
    }
  }
}

impl Display for Node<'_> {
  fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
    let mut level: usize = 0;
    self.fmt(f, &mut level)
  }
}

#[cfg(test)]
mod tests {
  use regex::Regex;

  use crate::node::Node;

  #[test]
  fn instantiates_node() {
    assert_eq!(
      Node {
        name: "Node",
        text: "a",
        column: 1,
        row: 0,
        children: Vec::new()
      },
      Node::new("Node", "a", 1, 0)
    );
  }

  #[test]
  fn instantiates_node_with_same_name_and_text() {
    assert_eq!(Node::new("a", "a", 1, 0), Node::of("a", 1, 0));
  }

  #[test]
  fn branches_to_another_node() {
    assert_eq!(
      Node {
        name: "Node #1",
        text: "a",
        column: 1,
        row: 0,
        children: vec![Some(Node::new("b", "b", 1, 1))]
      },
      Node::new("Node #1", "a", 1, 0).branch_to("b", |node| Ok(node)).unwrap()
    )
  }

  #[test]
  fn branches_to_another_node_with_the_given_name() {
    assert_eq!(
      Node {
        name: "Node #1",
        text: "a",
        column: 1,
        row: 0,
        children: vec![Some(Node::new("Node #2", "b", 1, 1))]
      },
      Node::new("Node #1", "a", 1, 0).branch_to_named("Node #2", "b", |node| Ok(node)).unwrap()
    )
  }

  #[test]
  fn branches_to_another_node_with_the_given_name_whose_text_matches_the_pattern() {
    assert_eq!(
      Node {
        name: "Node #1",
        text: "a",
        column: 1,
        row: 0,
        children: vec![Some(Node::new("Node #2", "b", 1, 1))]
      },
      Node::new("Node #1", "a", 1, 0)
        .branch_to_named_and_patterned(
          "Node #2",
          Regex::new(r"b").unwrap(),
          || String::from("Not a \"b\"!"),
          "b",
          |node| Ok(node)
        )
        .unwrap()
    )
  }

  #[test]
  fn errors_when_branching_to_another_node_with_the_given_name_whose_text_does_not_match_the_pattern(
  ) {
    Node::new("Node #1", "a", 1, 0)
      .branch_to_named_and_patterned(
        "Node #2",
        Regex::new(r"b").unwrap(),
        || String::from("Not a \"b\"!"),
        "c",
        |node| Ok(node)
      )
      .expect_err("Not a \"b\"!");
  }
}
